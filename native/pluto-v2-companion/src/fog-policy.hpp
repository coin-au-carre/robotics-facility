#pragma once
#include <cstdint>
#include <stdexcept>

namespace pluto_fog {
// Undetected foreign units have no model-visible identity. Own cloaked or loaded
// units remain available; foreign units become available when detection returns.
inline bool expose_unit(uint8_t owner, uint32_t self, uint32_t flags, uint32_t detection) {
  return self < 8 && (owner == self || !(flags & 0x100u) || (detection & (1u << self)));
}

// Sprite visibility is the projectile's public observation boundary. A missing
// sprite does not establish foreign visibility; only own sprite-less shots remain.
inline bool expose_projectile(uint8_t owner, uint32_t self, bool has_sprite, uint8_t visibility) {
  return self < 8 && (has_sprite ? (visibility & (1u << self)) != 0 : owner == self);
}

// Rewrite only copied list nodes. A dropped node must not truncate later visible
// units, and no copied next/previous pointer may still reference that node.
template<class Next, class Translate, class Link>
uint32_t project_list(uint32_t raw, Next next, Translate translate, Link link) {
  uint32_t first = 0, previous = 0, before_previous = 0;
  unsigned visited = 0;
  while (raw) {
    if (++visited > 8192) throw std::runtime_error("Invalid Pluto unit list");
    const auto following = next(raw);
    const auto mapped = translate(raw);
    if (mapped) {
      if (!first) first = mapped;
      link(mapped, previous, 0);
      if (previous) link(previous, before_previous, mapped);
      before_previous = previous; previous = mapped;
    }
    raw = following;
  }
  return first;
}
}
