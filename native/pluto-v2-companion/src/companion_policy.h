#pragma once
#include "companion_api.h"
#include "session.h"

// Independent admission/sequence gate: rejection never reads or mutates game
// memory. A rewind poisons the epoch instead of restarting external inference.
class CompanionPolicy {
public:
  int32_t start(const PlutoFrameV1& f, const Session& session) {
    if(active || failed || epoch!=0 || f.epoch==0 || f.frame!=0 || !valid(f) ||
       session_rejection(session,true)!=nullptr || !session.multiplayer ||
       session.self!=f.local_player) return PLUTO_REJECTED;
    for(unsigned i=0;i<8;++i)if(session.players[i]==1)return PLUTO_REJECTED;
    epoch=f.epoch;frame=0;self=f.local_player;active=true;return PLUTO_OK;
  }
  int32_t next(const PlutoFrameV1& f) {
    if(!active || f.epoch!=epoch)return PLUTO_BAD_STATE;
    if(!valid(f) || f.local_player!=self || f.frame<frame || f.frame>frame+1) {
      active=false;failed=true;return PLUTO_REJECTED;
    }
    if(f.frame==frame)return PLUTO_SKIPPED;
    frame=f.frame;return PLUTO_OK;
  }
  int32_t end(uint32_t e) {
    if(epoch==0 || e!=epoch)return PLUTO_BAD_STATE;
    if(ended)return PLUTO_SKIPPED;
    // A poisoned command stream still owes one final result callback.
    ended=true;active=false;return PLUTO_OK;
  }
  void poison(){active=false;failed=true;}
  bool running() const{return active && !failed;}
  static bool valid(const PlutoFrameV1& f) {
    return f.size==sizeof(PlutoFrameV1) && f.version==PLUTO_ABI_V1 &&
      f.flags==(PLUTO_PRIVATE|PLUTO_MULTIPLAYER) && f.local_player<8 &&
      f.latency_frames>=1 && f.latency_frames<=64 && f.frame<0x7fffffffu;
  }
private:
  uint32_t epoch=0,frame=0,self=8;
  bool active=false,failed=false,ended=false;
};
