#pragma once

#include <FastLED.h>
#include <vector>

namespace animations {

class Animation {
private:
  bool enabled;
  const String name;
  const long duration;

protected:
  Animation(const String name, const long duration)
      : enabled(false), name(name), duration(duration) {}

public:
  const String get_name() { return name; }

  long get_duration() { return duration; }

  virtual CRGB get_pixel(const uint8_t x, const uint8_t y, const long t) = 0;

  virtual bool get_enabled() { return enabled; }

  void set_enabled(bool e) { enabled = e; }
};

std::vector<Animation *> get_animations();

} // namespace animations
