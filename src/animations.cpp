#include "./animations.h"
#include "./chars.h"
#include "./ha.h"

namespace animations {

class OffAnimation : public Animation {
public:
  OffAnimation() : Animation("Off", 1) {}

  bool get_enabled() { return true; }

  CRGB get_pixel(const uint8_t x, const uint8_t y, const long t) {
    return CRGB::Black;
  }
};

class OnAirAnimation : public Animation {
private:
  const String on_air_string = "On Air";

public:
  OnAirAnimation() : Animation("On Air", (6 * CHARS_WIDTH) + 64) {}

  CRGB get_pixel(const uint8_t x, const uint8_t y, const long t) {
    const uint8_t v_x = (x + t) - 32;
    const uint8_t char_index = v_x / CHARS_WIDTH;
    if (char_index >= on_air_string.length()) {
      return CRGB::Black;
    }

    uint8_t val = CHARS[on_air_string.charAt(char_index)][v_x % CHARS_WIDTH][y];

    return CHSV(0, 255, val);
  }
};

std::vector<Animation *> animation_list{
    new OffAnimation(),
    new OnAirAnimation(),
};

std::vector<Animation *> get_animations() { return animation_list; }

} // namespace animations
