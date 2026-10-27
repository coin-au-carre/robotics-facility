#pragma once
#include <stdint.h>
#include <wchar.h>

// Versioned C ABI. All calls and callbacks use the same game thread. No STL,
// BWAPI objects or ownership of allocated buffers cross this boundary.
#if defined(_MSC_VER)
#define PLUTO_CALL __cdecl
#else
#define PLUTO_CALL
#endif
#ifdef __cplusplus
extern "C" {
#endif
enum { PLUTO_ABI_V1 = 1 };
enum PlutoStatus {
  PLUTO_OK=0, PLUTO_SKIPPED=1, PLUTO_SESSION_ENDED=2, PLUTO_BAD_ABI=-1, PLUTO_REJECTED=-2,
  PLUTO_BAD_STATE=-3, PLUTO_FAILURE=-4, PLUTO_WRONG_THREAD=-5
};
enum PlutoFlags {
  PLUTO_PRIVATE=1, PLUTO_MULTIPLAYER=2, PLUTO_REPLAY=4, PLUTO_ROLLBACK=8
};
typedef struct PlutoHostV1 {
  uint32_t size, version;
  void* user;
  int32_t (PLUTO_CALL *submit_packet)(void*, const uint8_t*, uint32_t);
  void (PLUTO_CALL *print_local)(void*, const char*);
  void (PLUTO_CALL *request_leave)(void*);
} PlutoHostV1;
typedef struct PlutoConfigV1 {
  uint32_t size, version;
  const wchar_t* bot_path;
  const wchar_t* log_path;
} PlutoConfigV1;
typedef struct PlutoFrameV1 {
  uint32_t size, version, epoch, frame, local_player, latency_frames, flags;
} PlutoFrameV1;
typedef int32_t (PLUTO_CALL *PlutoInitializeFn)(const PlutoHostV1*, const PlutoConfigV1*);
typedef int32_t (PLUTO_CALL *PlutoFrameFn)(const PlutoFrameV1*);
typedef int32_t (PLUTO_CALL *PlutoEndFn)(uint32_t epoch, uint32_t won);
typedef int32_t (PLUTO_CALL *PlutoShutdownFn)(void);
#ifdef __cplusplus
}
#endif
