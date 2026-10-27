#include "legacy_view.h"
#include "pluto_bindings.h"
#include "scr_profile_13515_x86.h"
#include "scr_layout.h"
#include "commands.h"
#include "fog-policy.hpp"
#include <BWAPI.h>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <unordered_set>

namespace {
template<class T> void put(uintptr_t p,T value){std::memcpy(reinterpret_cast<void*>(p),&value,sizeof(value));}
// Populate documented scalar/array fields. Opaque game bytes and foreign private
// tables are never copied into the model's legacy address space.
void copy_public_game(uint32_t dest,uint32_t raw,unsigned self) {
  std::memset(reinterpret_cast<void*>(dest),0,layout::Game::size);
  const auto copy=[&](size_t offset,size_t length) {
    std::memcpy(reinterpret_cast<void*>(dest+offset),reinterpret_cast<const void*>(raw+offset),length);
  };
  for(auto range:{std::pair<size_t,size_t>{192,40},{236,4},{241,3},{332,8},{374,1},{3148,292},
      {58560,132},{58884,8},{58896,8},{66384,1}})copy(range.first,range.second);
  // Start positions form a public set, but native player-to-position assignment
  // is private. Preserve own position and canonicalize every other table slot.
  std::array<uint32_t,7> starts{};unsigned count=0;
  for(unsigned player=0;player<8;++player)if(player!=self)
    starts[count++]=scr::read<uint32_t>(raw+layout::Game::start_position+player*4);
  std::sort(starts.begin(),starts.end());count=0;
  for(unsigned player=0;player<8;++player)
    put<uint32_t>(dest+layout::Game::start_position+player*4,player==self?
      scr::read<uint32_t>(raw+layout::Game::start_position+player*4):starts[count++]);
  for(size_t table:{0u,48u,96u,144u,12804u})copy(table+self*4,4);
  for(unsigned category=0;category<18;++category)copy(layout::Game::scores+category*48+self*4,4);
  for(unsigned race=0;race<3;++race)for(unsigned field=0;field<3;++field)
    copy(layout::Game::supplies+race*144+field*48+self*4,4);
  for(size_t table:{layout::Game::all_units_count,layout::Game::completed_units_count,
      layout::Game::unit_kills,layout::Game::deaths})
    for(unsigned type=0;type<228;++type)copy(table+(type*12+self)*4,4);
  for(auto table:{std::pair<size_t,size_t>{layout::Game::unit_availability,228},
      {layout::Game::tech_availability_sc,24},{layout::Game::tech_level_sc,24},
      {layout::Game::tech_in_progress_legacy,3},{layout::Game::upgrade_limit_sc,46},
      {layout::Game::upgrade_level_sc,46},{layout::Game::tech_availability_bw,20},
      {layout::Game::tech_level_bw,20},{layout::Game::tech_in_progress,6},
      {layout::Game::upgrade_limit_bw,15},{layout::Game::upgrade_level_bw,15},
      {layout::Game::upgrade_in_progress,8}})copy(table.first+self*table.second,table.second);
  for(unsigned other=0;other<12;++other) {
    copy(layout::Game::alliances+self*12+other,1);
    copy(layout::Game::alliances+other*12+self,1);
  }
}
thread_local LegacyView* current_view=nullptr;
}
LegacyView::LegacyView():memory(last-first){}
void LegacyView::reset() {
  std::fill(memory.begin(),memory.end(),uint8_t{0});slots={};sprites={};spell_sprites={};race_known={};lookup.clear();
  raw_start=raw_length=0;tile_flags.clear();flush_error.clear();sent=0;pending_turns.clear();
}
void LegacyView::begin_frame(FILE* log,int frame) {
  current_view=this;frame_log=log;frame_number=frame;sent=0;
  const auto send=reinterpret_cast<void (__cdecl*)(const uint8_t*,size_t)>(scr::send_command());
  for(auto turn=pending_turns.begin();turn!=pending_turns.end();) {
    if(turn->frame>=frame){++turn;continue;}
    for(const auto& packet:turn->packets) {
      if(packet_sink)packet_sink(packet.data(),packet.size());
      else send(packet.data(),packet.size());
      ++sent;
      std::fprintf(log,"{\"command_frame\":%d,\"queued_frame\":%d,\"packet_hex\":\"",frame,turn->frame);
      for(auto b:packet)std::fprintf(log,"%02x",b);
      std::fprintf(log,"\",\"submitted\":true}\n");
    }
    turn=pending_turns.erase(turn);
  }
}
void __cdecl LegacyView::flush_turn() noexcept {
  if(!current_view)return;
  try{current_view->drain(current_view->frame_log,current_view->frame_number);}
  catch(const std::exception& e){
    current_view->flush_error=e.what();
    put<uint32_t>(current_view->address(0x654aa0),0);
  }
}
uint32_t LegacyView::address(uint32_t old) const {
  if(old<first || old>=last)throw std::runtime_error("Out of range legacy address");
  return reinterpret_cast<uint32_t>(memory.data())+old-first;
}
uint32_t LegacyView::unit(uint32_t raw) const {
  auto found=lookup.find(raw);
  return found==lookup.end()?0:address(unit_base+found->second*336);
}
uint16_t LegacyView::handle_for_raw(uint32_t raw) const {
  auto found=lookup.find(raw);if(found==lookup.end())return 0;
  auto index=found->second;return static_cast<uint16_t>((index+1)|(slots[index].generation<<11));
}
uint32_t LegacyView::scr_handle(uint16_t id) const {
  unsigned index=id&0x7ff;if(!index || index>slots.size())return 0;--index;
  auto& slot=slots[index];if(!slot.raw || slot.generation!=(id>>11))return 0;
  return (slot.raw-raw_start)/336+1 | (uint32_t(slot.source_generation)<<(raw_length>1700?13:11));
}
void LegacyView::sprite(uint32_t dest,uint32_t raw) {
  std::memset(reinterpret_cast<void*>(dest),0,36);
  std::memcpy(reinterpret_cast<void*>(dest+8),reinterpret_cast<void*>(raw+8),3);
  put<uint8_t>(dest+13,scr::read<uint8_t>(raw+13));
  put<uint8_t>(dest+14,static_cast<uint8_t>(scr::read<uint8_t>(raw+14)&0x60));
  put<uint32_t>(dest,0);put<uint32_t>(dest+4,0);
  // Other players' visibility is private in normal non-replay observations.
  put<uint8_t>(dest+12,static_cast<uint8_t>(scr::read<uint8_t>(raw+12)&(1u<<scr::local_player_id())));
  put<uint16_t>(dest+20,static_cast<uint16_t>(scr::read<uint32_t>(raw+20)));
  put<uint16_t>(dest+22,static_cast<uint16_t>(scr::read<uint32_t>(raw+24)));
  // CImage graphs belong to the native engine and are not observation data.
  put<uint16_t>(dest+16,0);
  std::memset(reinterpret_cast<void*>(dest+24),0,12);
}
void LegacyView::update() {
  raw_start=scr::units();raw_length=scr::read<uint32_t>(scr::addr(0x10436f4));
  if(!raw_start || !raw_length || raw_length>8192)throw std::runtime_error("Invalid SCR unit vector");
  std::unordered_set<uint32_t> present;
  for(unsigned i=0;i<raw_length;++i) {
    auto raw=raw_start+i*336;
    const auto sprite=scr::read<uint32_t>(raw+12);const auto owner=scr::read<uint8_t>(raw+76);
    const auto self=scr::local_player_id();
    if(sprite && owner<12 && scr::read<uint16_t>(raw+100)<228 &&
       !(scr::read<uint8_t>(raw+77)==0 && scr::read<uint8_t>(raw+78)==1) &&
       (owner==self || (scr::read<uint8_t>(sprite+12)&(1u<<self))) &&
       pluto_fog::expose_unit(scr::read<uint8_t>(raw+76),scr::local_player_id(),
         scr::read<uint32_t>(raw+220),scr::read<uint32_t>(raw+228))) {
      present.insert(raw);
    }
  }
  // Allocate in public traversal order, independent of native vector addresses
  // and omitted actors. Include eligible attached units not on an engine root.
  std::vector<uint32_t> public_order;
  std::unordered_set<uint32_t> reached;
  const auto append=[&](uint32_t raw) {
    if(present.count(raw) && reached.insert(raw).second)public_order.push_back(raw);
  };
  for(auto head:{scr::first_active_unit(),scr::first_hidden_unit(),scr::read<uint32_t>(scr::addr(0x10436b8))}) {
    unsigned visited=0;
    for(auto raw=head;raw;raw=scr::read<uint32_t>(raw+4)) {
      if(++visited>8192 || raw<raw_start || (raw-raw_start)%336 || (raw-raw_start)/336>=raw_length)
        throw std::runtime_error("Invalid SCR allocation list");
      append(raw);
    }
  }
  for(size_t i=0;i<public_order.size();++i) {
    const auto raw=public_order[i];const BWAPI::UnitType type(scr::read<uint16_t>(raw+100));
    for(unsigned offset:{20u,92u,112u,128u,236u})append(scr::read<uint32_t>(raw+offset));
    if(type.isBuilding() || type.isWorker() || type==BWAPI::UnitTypes::Protoss_Interceptor || type==BWAPI::UnitTypes::Protoss_Scarab)
      append(scr::read<uint32_t>(raw+192));
    if(type==BWAPI::UnitTypes::Zerg_Nydus_Canal || type==BWAPI::UnitTypes::Terran_Nuclear_Silo)
      append(scr::read<uint32_t>(raw+208));
    if(scr::read<uint8_t>(raw+76)==scr::local_player_id()) {
      if(type==BWAPI::UnitTypes::Protoss_Carrier || type==BWAPI::UnitTypes::Hero_Gantrithor ||
         type==BWAPI::UnitTypes::Protoss_Reaver || type==BWAPI::UnitTypes::Hero_Warbringer) {
        append(scr::read<uint32_t>(raw+192));append(scr::read<uint32_t>(raw+196));
      }
      if(type==BWAPI::UnitTypes::Protoss_Interceptor || type==BWAPI::UnitTypes::Protoss_Scarab) {
        append(scr::read<uint32_t>(raw+196));append(scr::read<uint32_t>(raw+200));
      }
      if(type.canProduce())append(scr::read<uint32_t>(raw+252));
      const auto high=raw_length>1700?scr::read<uint16_t>(raw+306):0;
      for(unsigned n=0;n<8;++n) {
        const uint32_t id=scr::read<uint16_t>(raw+176+n*2)|(((high>>(n*2))&3)<<16);
        const auto index=id&(raw_length>1700?0x1fff:0x7ff);
        if(index && index<=raw_length) {
          const auto loaded=raw_start+(index-1)*336;
          if(scr::read<uint8_t>(loaded+165)==(id>>(raw_length>1700?13:11)))append(loaded);
        }
      }
    }
  }
  present=std::move(reached);
  // Race knowledge comes from live lists, not stale sprite-bearing vector slots.
  for(auto head:{scr::first_active_unit(),scr::first_hidden_unit()}) {
    unsigned visited=0;
    for(auto raw=head;raw;raw=scr::read<uint32_t>(raw+4)) {
      if(++visited>8192 || raw<raw_start || (raw-raw_start)%336 || (raw-raw_start)/336>=raw_length)
        throw std::runtime_error("Invalid SCR race-observation list");
      if(!present.count(raw) || (scr::read<uint8_t>(raw+77)==0 && scr::read<uint8_t>(raw+78)==1))continue;
      const auto owner=scr::read<uint8_t>(raw+76);const auto self=scr::local_player_id();
      const auto sprite=scr::read<uint32_t>(raw+12);
      if(owner<8 && self<8 && (scr::read<uint8_t>(sprite+12)&(1u<<self)))race_known[owner]=true;
    }
  }
  if(present.size()>slots.size())throw std::runtime_error("Pluto's legacy unit view exceeded 1700 simultaneous units");
  for(unsigned i=0;i<slots.size();++i)if(slots[i].raw && !present.count(slots[i].raw)) {
    lookup.erase(slots[i].raw);slots[i].raw=0;
    std::memset(reinterpret_cast<void*>(address(unit_base+i*336)),0,336);
  }
  for(auto raw:public_order) {
    const auto gen=scr::read<uint8_t>(raw+165);auto found=lookup.find(raw);
    if(found==lookup.end()) {
      auto empty=std::find_if(slots.begin(),slots.end(),[](auto& s){return s.raw==0;});
      auto index=static_cast<unsigned>(empty-slots.begin());
      empty->raw=raw;empty->source_generation=gen;empty->generation=(empty->generation+1)&31;lookup.emplace(raw,index);
    }else if(slots[found->second].source_generation!=gen) {
      auto& slot=slots[found->second];slot.source_generation=gen;slot.generation=(slot.generation+1)&31;
    }
  }
  const auto self=scr::local_player_id();
  if(self>=8)throw std::runtime_error("Invalid public-view player");
  copy_public_game(address(0x57f0f0),scr::game(),self);
  std::memset(reinterpret_cast<void*>(address(0x57eee0)),0,12*36);
  for(unsigned player=0;player<12;++player) {
    const auto dest=address(0x57eee0+player*36),raw=scr::players()+player*36;
    std::memcpy(reinterpret_cast<void*>(dest),reinterpret_cast<const void*>(raw),4);
    std::memcpy(reinterpret_cast<void*>(dest+8),reinterpret_cast<const void*>(raw+8),28);
  } // Storm network IDs are engine metadata, not player observation fields.
  // Public lobby selections are unavailable. Match the snapshot's conservative
  // race knowledge in both player metadata and resolved starting-race bytes.
  for(unsigned i=0;i<8;++i)if(i!=scr::local_player_id() && !race_known[i]) {
    put<uint8_t>(address(0x57eee0+i*36+layout::Player::race),8);
    put<uint8_t>(address(0x57f0f0+layout::Game::starting_races+i),8);
  }
  put<uint32_t>(address(0x512688),scr::local_player_id());
  put<uint32_t>(address(0x59688c),scr::is_multiplayer()?1u:0u);put<uint16_t>(address(0x596904),2);
  const auto width=scr::read<uint16_t>(scr::game()+layout::Game::map_width_tiles);
  const auto height=scr::read<uint16_t>(scr::game()+layout::Game::map_height_tiles);
  if(!width || width>128 || !height || height>128 || !scr::map_tile_flags())
    throw std::runtime_error("Invalid public-view terrain");
  tile_flags.resize(size_t(width)*height);
  const uint32_t own_bits=(1u<<self)|(1u<<(self+8));
  constexpr uint32_t static_bits=0x27850000u; // walkability, buildability, height and cliff
  constexpr uint32_t dynamic_bits=0x58400000u; // creep, occupancy and receding creep
  for(size_t i=0;i<tile_flags.size();++i) {
    const auto native=scr::read<uint32_t>(scr::map_tile_flags()+i*4);
    tile_flags[i]=(native&(static_bits|own_bits))|(0xffffu&~own_bits);
    if(!(native&(1u<<self)))tile_flags[i]|=native&dynamic_bits;
  }
  put<uint32_t>(address(0x6d1260),reinterpret_cast<uint32_t>(tile_flags.data()));
  for(unsigned i=0;i<slots.size();++i)if(slots[i].raw) {
    const auto raw=slots[i].raw,dest=address(unit_base+i*336);
    // Explicit scalar schema: all other bytes remain zero. Typed unions,
    // local identity and supported relations are populated separately below.
    std::memset(reinterpret_cast<void*>(dest),0,336);
    const auto copy=[&](unsigned offset,unsigned length) {
      std::memcpy(reinterpret_cast<void*>(dest+offset),reinterpret_cast<const void*>(raw+offset),length);
    };
    for(auto range:{std::pair<unsigned,unsigned>{8,4},{16,4},{33,1},{40,4},{64,8},
        {76,2},{84,8},{96,6},{142,3},{148,2},{166,1},{272,12},{292,1},{294,1}})copy(range.first,range.second);
    put<uint8_t>(dest+32,static_cast<uint8_t>(scr::read<uint8_t>(raw+32)&0x1e));
    put<uint8_t>(dest+151,scr::read<uint8_t>(raw+151)==13?13:0);
    const bool attacked=scr::read<uint8_t>(raw+139)==174 && scr::read<uint8_t>(raw+138)!=0;
    put<uint8_t>(dest+138,attacked?1:0);put<uint8_t>(dest+139,attacked?174:0);
    put<uint8_t>(dest+289,scr::read<uint8_t>(raw+289)!=0?1:0);
    put<uint8_t>(dest+291,scr::read<uint8_t>(raw+291)!=0?1:0);
    constexpr uint32_t public_status=0x1u|0x2u|0x4u|0x8u|0x10u|0x100u|0x200u|0x400u|0x1000u|0x800000u|0x4000000u;
    const bool owned=scr::read<uint8_t>(raw+76)==self;
    auto status=scr::read<uint32_t>(raw+220)&public_status;
    // Ordinary BWAPI never discloses enemy hallucinations. Also normalize its
    // inverse IsNormal bit; masking only the positive bit leaves the same leak.
    status|=owned?(scr::read<uint32_t>(raw+220)&(0x60u|0x100000u|0x40000000u)):0x100000u;
    put<uint32_t>(dest+220,status);
    for(unsigned q=0;q<5;++q)put<uint16_t>(dest+152+q*2,228);
    if(owned){copy(133,1);copy(152,13);copy(172,2);}
    put<uint32_t>(dest+layout::Unit::detection_status,
      scr::read<uint32_t>(raw+layout::Unit::detection_status)&(1u<<scr::local_player_id()));
    for(unsigned offset:{0,4,20,92,112,236})put<uint32_t>(dest+offset,unit(scr::read<uint32_t>(raw+offset)));
    // Engine-owned graph pointers are never exposed. Known CUnit relations are
    // rewritten through the visible/owned slot map; scalar unions stay scalar.
    for(unsigned offset:{116u,120u,256u,308u})put<uint32_t>(dest+offset,0);
    put<uint8_t>(dest+132,0);put<uint8_t>(dest+146,0);

    std::memset(reinterpret_cast<void*>(dest+312),0,20);
    const BWAPI::UnitType unit_type(scr::read<uint16_t>(raw+100));
    const auto copy_scalar=[&](unsigned offset,unsigned length) {
      std::memcpy(reinterpret_cast<void*>(dest+offset),reinterpret_cast<const void*>(raw+offset),length);
    };
    const auto relation=[&](unsigned offset){put<uint32_t>(dest+offset,unit(scr::read<uint32_t>(raw+offset)));};
    std::memset(reinterpret_cast<void*>(dest+192),0,28);
    if(unit_type.isResourceContainer()) {
      copy_scalar(208,2);copy_scalar(216,1);
      put<uint8_t>(dest+211,(scr::read<uint8_t>(raw+211)!=0 || scr::read<uint32_t>(raw+212)!=0)?1:0);
    }else if(unit_type.isWorker()) {
      relation(192);copy_scalar(224,1);
      if(owned){copy_scalar(196,4);relation(200);copy_scalar(204,4);relation(208);}
    }else if(unit_type==BWAPI::UnitTypes::Protoss_Carrier || unit_type==BWAPI::UnitTypes::Hero_Gantrithor ||
             unit_type==BWAPI::UnitTypes::Protoss_Reaver || unit_type==BWAPI::UnitTypes::Hero_Warbringer) {
      copy_scalar(200,2);
    }else if(unit_type==BWAPI::UnitTypes::Protoss_Interceptor || unit_type==BWAPI::UnitTypes::Protoss_Scarab) {
      relation(192);if(owned)copy_scalar(204,1);
    }else if(unit_type.isBuilding()) {
      relation(192);
      if(owned)copy_scalar(196,7);
      if(unit_type==BWAPI::UnitTypes::Zerg_Nydus_Canal || unit_type==BWAPI::UnitTypes::Terran_Nuclear_Silo)relation(208);
      if(unit_type==BWAPI::UnitTypes::Terran_Nuclear_Silo && scr::read<uint8_t>(raw+76)==self)copy_scalar(212,1);

    }else if(unit_type==BWAPI::UnitTypes::Terran_Vulture || unit_type==BWAPI::UnitTypes::Hero_Jim_Raynor_Vulture)copy_scalar(192,1);
    if(owned || unit_type==BWAPI::UnitTypes::Zerg_Larva)relation(128);
    if(owned && unit_type.canProduce()){copy_scalar(248,4);relation(252);}
    if(scr::read<uint8_t>(raw+76)!=self) {
      for(unsigned q=0;q<5;++q)put<uint16_t>(dest+152+q*2,228);
      put<uint16_t>(dest+162,0);put<uint8_t>(dest+164,0);put<uint16_t>(dest+172,0);
      if(unit_type!=BWAPI::UnitTypes::Protoss_Pylon)put<uint32_t>(dest+252,0);
    }
    auto raw_sprite=scr::read<uint32_t>(raw+12);sprite(reinterpret_cast<uint32_t>(sprites[i].data()),raw_sprite);
    put<uint32_t>(dest+12,reinterpret_cast<uint32_t>(sprites[i].data()));put<uint8_t>(dest+165,slots[i].generation);
    const auto type=scr::read<uint16_t>(raw+100);
    if(owned && (type==1 || type==16 || type==99 || type==100 || type==104)) {
      const auto target=scr::read<uint32_t>(raw+208);
      if(target && (scr::read<uint8_t>(target+12)&(1u<<self))) {
        const auto mapped=reinterpret_cast<uint32_t>(spell_sprites[i].data());
        sprite(mapped,target);put<uint32_t>(dest+208,mapped);
      }else spell_sprites[i]={};
    }
    const auto high=raw_length>1700?scr::read<uint16_t>(raw+306):0;
    for(unsigned n=0;n<8;++n) {
      const uint32_t id=scr::read<uint16_t>(raw+176+n*2)|(((high>>(n*2))&3)<<16);
      const uint32_t index=id&(raw_length>1700?0x1fff:0x7ff);
      uint16_t legacy=0;
      if(owned && index && index<=raw_length) {
        auto loaded=raw_start+(index-1)*336;
        if(scr::read<uint8_t>(loaded+165)==(id>>(raw_length>1700?13:11)))legacy=handle_for_raw(loaded);
      }
      put<uint16_t>(dest+176+n*2,legacy);
    }
  }
  // Player/psi chains contain no native predecessor-dependent endpoints.
  std::array<uint32_t,12> player_tail{};uint32_t pylon_tail=0;
  for(auto raw:public_order) {
    const auto mapped=unit(raw);const auto owner=scr::read<uint8_t>(raw+76);
    put<uint32_t>(mapped+104,player_tail[owner]);
    if(player_tail[owner])put<uint32_t>(player_tail[owner]+108,mapped);
    player_tail[owner]=mapped;
    if(owner==self && scr::read<uint16_t>(raw+100)==BWAPI::UnitTypes::Protoss_Pylon) {
      put<uint32_t>(mapped+248,pylon_tail);
      if(pylon_tail)put<uint32_t>(pylon_tail+252,mapped);
      pylon_tail=mapped;
    }
  }
  const auto project=[&](uint32_t head) {
    return pluto_fog::project_list(head,[&](uint32_t raw) {
      if(raw<raw_start || (raw-raw_start)%336 || (raw-raw_start)/336>=raw_length)
        throw std::runtime_error("Invalid SCR unit list pointer");
      return scr::read<uint32_t>(raw+4);
    },[&](uint32_t raw){return unit(raw);},[&](uint32_t mapped,uint32_t previous,uint32_t next) {
      put<uint32_t>(mapped,previous);put<uint32_t>(mapped+4,next);
    });
  };
  put<uint32_t>(address(0x628430),project(scr::first_active_unit()));
  put<uint32_t>(address(0x6283ec),project(scr::first_hidden_unit()));
  put<uint32_t>(address(0x6283f4),project(scr::read<uint32_t>(scr::addr(0x10436b8))));
  std::vector<uint32_t> raw_bullets;
  std::unordered_set<uint32_t> visited_bullets;
  for(auto bullet=scr::read<uint32_t>(scr::addr(0x1034a78));bullet;bullet=scr::read<uint32_t>(bullet+4)) {
    if(visited_bullets.size()>=8192 || !visited_bullets.insert(bullet).second)throw std::runtime_error("Invalid SCR bullet list");
    const auto raw_sprite=scr::read<uint32_t>(bullet+12);
    if(pluto_fog::expose_projectile(scr::read<uint8_t>(bullet+76),scr::local_player_id(),
         raw_sprite!=0,raw_sprite?scr::read<uint8_t>(raw_sprite+12):0))raw_bullets.push_back(bullet);
  }
  bullets.resize(raw_bullets.size());bullet_sprites.resize(raw_bullets.size());
  const auto bullet_address=[this](size_t index){return reinterpret_cast<uint32_t>(bullets[index].data());};
  put<uint32_t>(address(0x64dec4),bullets.empty()?0:bullet_address(0));
  for(size_t count=0;count<raw_bullets.size();++count) {
    const auto bullet=raw_bullets[count],dest=bullet_address(count);
    std::memset(reinterpret_cast<void*>(dest),0,112);
    for(auto range:{std::pair<unsigned,unsigned>{16,4},{24,4},{33,1},{36,2},{40,4},{64,8},
        {74,1},{76,1},{88,4},{96,2}})
      std::memcpy(reinterpret_cast<void*>(dest+range.first),reinterpret_cast<const void*>(bullet+range.first),range.second);
    put<uint32_t>(dest+8,1);
    put<uint32_t>(dest,count?bullet_address(count-1):0);
    put<uint32_t>(dest+4,count+1<bullets.size()?bullet_address(count+1):0);
    for(unsigned offset:{20,92,100})put<uint32_t>(dest+offset,unit(scr::read<uint32_t>(bullet+offset)));
    auto raw_sprite=scr::read<uint32_t>(bullet+12);
    if(raw_sprite){
      const auto sprite_address=reinterpret_cast<uint32_t>(bullet_sprites[count].data());
      sprite(sprite_address,raw_sprite);put<uint32_t>(dest+12,sprite_address);
    }
  }
}
void LegacyView::bind(HMODULE pluto) {
  const auto base=reinterpret_cast<uint32_t>(pluto);
  for(auto& binding:pluto_bindings)if(scr::read<uint32_t>(base+binding.rva)!=binding.expected)throw std::runtime_error("Pluto binding validation failed; unsupported binary");
  for(auto& binding:pluto_bindings) {
    const int32_t signed_address=static_cast<int32_t>(binding.expected);
    uint32_t value;
    if(binding.expected==0x485a40)value=reinterpret_cast<uint32_t>(&LegacyView::flush_turn);
    else if(signed_address<0)value=0u-address(static_cast<uint32_t>(-signed_address));
    else value=address(binding.expected);
    DWORD old;
    auto at=reinterpret_cast<void*>(base+binding.rva);
    if(!VirtualProtect(at,4,PAGE_EXECUTE_READWRITE,&old))throw std::runtime_error("Cannot bind Pluto binary");
    put<uint32_t>(base+binding.rva,value);
    DWORD unused;VirtualProtect(at,4,old,&unused);
  }
  FlushInstructionCache(GetCurrentProcess(),nullptr,0);
}
unsigned LegacyView::drain(FILE*,int frame) {
  if(!flush_error.empty())throw std::runtime_error(flush_error);
  const auto size=scr::read<uint32_t>(address(0x654aa0));
  if(size>512)throw std::runtime_error("Legacy command queue overflow");
  if(size){
    auto packets=translate_commands(reinterpret_cast<const uint8_t*>(address(0x654880)),size,[this](uint16_t id){return scr_handle(id);});
    // SCR's offline turn queue applies commands one frame earlier than Pluto's
    // 1.16.1 training environment. Retain one frame to preserve the measured
    // four-frame observation-to-effect delay, including selection ordering.
    pending_turns.push_back({frame,std::move(packets)});
  }
  put<uint32_t>(address(0x654aa0),0);
  return sent;
}
