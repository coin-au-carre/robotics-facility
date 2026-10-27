#pragma once
#include <Windows.h>
#include <array>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <string>
#include <functional>
#include <utility>
#include "commands.h"

// Compatibility view for the pinned Pluto binary's direct 1.16.1 memory reads.
// The bot writes only to this view; commands are collected for SCR translation.
class LegacyView {
public:
  LegacyView();
  void update();
  void bind(HMODULE pluto);
  void reset();
  void begin_frame(FILE* log,int frame);
  using PacketSink=std::function<void(const uint8_t*,size_t)>;
  void set_packet_sink(PacketSink sink){packet_sink=std::move(sink);}
  void discard_pending_turns(){pending_turns.clear();}
  unsigned drain(FILE* log,int frame);
  static void __cdecl flush_turn() noexcept;
  uint32_t scr_handle(uint16_t legacy_handle) const;
private:
  static constexpr uint32_t first=0x512000,last=0x6d2000,unit_base=0x59cca8;
  std::vector<uint8_t> memory;
  std::vector<uint32_t> tile_flags;
  std::array<std::array<uint8_t,36>,1700> sprites{};
  std::array<std::array<uint8_t,36>,1700> spell_sprites{};
  std::vector<std::array<uint8_t,112>> bullets;
  std::vector<std::array<uint8_t,36>> bullet_sprites;
  struct Slot {uint32_t raw=0;uint8_t source_generation=0,generation=0;};
  std::array<Slot,1700> slots{};
  std::array<bool,8> race_known{};
  std::unordered_map<uint32_t,unsigned> lookup;
  uint32_t raw_start=0,raw_length=0;
  FILE* frame_log=nullptr;
  int frame_number=0;
  std::string flush_error;
  unsigned sent=0;
  PacketSink packet_sink;
  struct PendingTurn {int frame;std::vector<Packet> packets;};
  std::vector<PendingTurn> pending_turns;
  uint32_t address(uint32_t old) const;
  uint32_t unit(uint32_t raw) const;
  uint16_t handle_for_raw(uint32_t raw) const;
  void sprite(uint32_t destination,uint32_t raw);
};
