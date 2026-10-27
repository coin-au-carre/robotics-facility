#include "companion_policy.h"
#include <stdexcept>
#include <cstdio>
static void check(bool value){if(!value)throw std::runtime_error("companion admission/sequence failure");}
int main(){
  Session s;s.multiplayer=true;s.self=3;s.game_type=2;s.players[3]=2;s.players[6]=2;
  PlutoFrameV1 f{sizeof(f),PLUTO_ABI_V1,1,0,3,4,PLUTO_PRIVATE|PLUTO_MULTIPLAYER};
  for(unsigned mask=0;mask<32;++mask){
    CompanionPolicy p;auto candidate=f;candidate.flags=mask;
    check((p.start(candidate,s)==PLUTO_OK)==(mask==f.flags));
  }
  for(unsigned slot=0;slot<8;++slot){
    Session v;v.multiplayer=true;v.self=slot;v.game_type=2;v.players[slot]=2;v.players[(slot+3)%8]=2;
    CompanionPolicy p;auto c=f;c.local_player=slot;check(p.start(c,v)==PLUTO_OK);
    check(p.next(c)==PLUTO_SKIPPED);c.frame=1;check(p.next(c)==PLUTO_OK);
    check(p.next(c)==PLUTO_SKIPPED);c.frame=0;check(p.next(c)==PLUTO_REJECTED);
    c.frame=2;check(p.next(c)==PLUTO_BAD_STATE);check(!p.running());
  }
  {CompanionPolicy p;check(p.start(f,s)==PLUTO_OK);auto gap=f;gap.frame=2;check(p.next(gap)==PLUTO_REJECTED);}
  {CompanionPolicy p;check(p.start(f,s)==PLUTO_OK);auto other=f;other.epoch=2;check(p.next(other)==PLUTO_BAD_STATE);
    check(p.end(1)==PLUTO_OK);check(p.end(1)==PLUTO_SKIPPED);check(p.start(other,s)==PLUTO_REJECTED);}
  {CompanionPolicy p;check(p.start(f,s)==PLUTO_OK);auto c=f;c.frame=1;c.flags|=PLUTO_ROLLBACK;check(p.next(c)==PLUTO_REJECTED);}
  {CompanionPolicy p;check(p.end(0)==PLUTO_BAD_STATE);check(p.end(1)==PLUTO_BAD_STATE);}
  {CompanionPolicy p;check(p.start(f,s)==PLUTO_OK);p.poison();
    check(!p.running());check(p.next(f)==PLUTO_BAD_STATE);check(p.end(2)==PLUTO_BAD_STATE);
    check(p.end(1)==PLUTO_OK);check(p.end(1)==PLUTO_SKIPPED);check(!p.running());}
  {CompanionPolicy p;check(p.start(f,s)==PLUTO_OK);auto gap=f;gap.frame=2;
    check(p.next(gap)==PLUTO_REJECTED);check(p.end(1)==PLUTO_OK);
    check(p.end(1)==PLUTO_SKIPPED);check(p.start(f,s)==PLUTO_REJECTED);}
  for(unsigned kind:{1u,3u,4u,10u,15u}){auto c=s;c.game_type=static_cast<uint16_t>(kind);CompanionPolicy p;check(p.start(f,c)==PLUTO_REJECTED);}
  {auto c=s;c.players[6]=1;CompanionPolicy p;check(p.start(f,c)==PLUTO_REJECTED);}
  {auto c=s;c.alliances[3][6]=1;CompanionPolicy p;check(p.start(f,c)==PLUTO_REJECTED);}
  {auto c=s;c.players[1]=2;CompanionPolicy p;check(p.start(f,c)==PLUTO_REJECTED);}
  for(unsigned latency:{0u,65u,0xffffffffu}){auto c=f;c.latency_frames=latency;CompanionPolicy p;check(p.start(c,s)==PLUTO_REJECTED);}
  std::puts("private lockstep 1v1 admission, all slots, duplicate/rewind/gap/epoch/rollback and one-match lifecycle passed");
}
