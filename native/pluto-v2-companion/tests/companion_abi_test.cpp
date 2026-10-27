#include <Windows.h>
#include "companion_api.h"
#include <cstdio>
#include <stdexcept>
#include <array>
#include <cstring>
static void check(bool v){if(!v)throw std::runtime_error("companion ABI fixture failed");}
static int32_t PLUTO_CALL submit(void*,const uint8_t*,uint32_t){return PLUTO_OK;}
static void PLUTO_CALL leave(void*){}
struct ThreadCall {PlutoFrameFn frame;const PlutoFrameV1* context;int32_t status=0;};
static DWORD WINAPI cross_thread(void* p){auto& c=*static_cast<ThreadCall*>(p);c.status=c.frame(c.context);return 0;}
int wmain(int argc,wchar_t** argv){
  check(argc==2);static_assert(sizeof(PlutoHostV1)==24);static_assert(sizeof(PlutoFrameV1)==28);
  static_assert(sizeof(PlutoConfigV1)==16);
  const auto timer=GetProcAddress(GetModuleHandleW(L"kernel32.dll"),"GetTickCount");
  std::array<unsigned char,32> before{};std::memcpy(before.data(),reinterpret_cast<const void*>(timer),before.size());
  auto dll=LoadLibraryW(argv[1]);check(dll!=nullptr);
  check(std::memcmp(before.data(),reinterpret_cast<const void*>(timer),before.size())==0);
  auto init=reinterpret_cast<PlutoInitializeFn>(GetProcAddress(dll,"PlutoInitialize"));
  auto start=reinterpret_cast<PlutoFrameFn>(GetProcAddress(dll,"PlutoMatchStart"));
  auto frame=reinterpret_cast<PlutoFrameFn>(GetProcAddress(dll,"PlutoFrame"));
  auto end=reinterpret_cast<PlutoEndFn>(GetProcAddress(dll,"PlutoMatchEnd"));
  auto shutdown=reinterpret_cast<PlutoShutdownFn>(GetProcAddress(dll,"PlutoShutdown"));
  check(init && start && frame && end && shutdown);
  // Loading DLL is inert: invalid ABI is rejected before hash/game-memory reads.
  check(init(nullptr,nullptr)==PLUTO_BAD_ABI);check(start(nullptr)==PLUTO_BAD_ABI);
  PlutoHostV1 host{sizeof(host),PLUTO_ABI_V1,nullptr,submit,nullptr,leave};
  PlutoConfigV1 config{sizeof(config),2,L"unused",L"unused"};
  check(init(&host,&config)==PLUTO_BAD_ABI);
  config.version=PLUTO_ABI_V1;
  // A valid ABI still cannot initialize inside this fixture's unsupported EXE.
  check(init(&host,&config)==PLUTO_FAILURE);
  PlutoFrameV1 context{sizeof(context),PLUTO_ABI_V1,1,1,3,4,PLUTO_PRIVATE|PLUTO_MULTIPLAYER};
  ThreadCall call{frame,&context};
  auto thread=CreateThread(nullptr,0,cross_thread,&call,0,nullptr);check(thread!=nullptr);
  check(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);CloseHandle(thread);
  check(call.status==PLUTO_WRONG_THREAD);
  check(frame(&context)==PLUTO_BAD_STATE);
  check(GetModuleHandleW(L"pluto.dll")==nullptr);
  check(std::memcmp(before.data(),reinterpret_cast<const void*>(timer),before.size())==0);
  check(shutdown()==PLUTO_OK);
  FreeLibrary(dll);std::puts("Win32 C ABI, unchanged timer, unsupported host, cross-thread and failed-state rejection passed");
}
