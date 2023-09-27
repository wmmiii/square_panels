#include <FastLED.h>

#include "./animations.h"
#include "./chars.h"
#include "./ha.h"
#include "./wifi.h"

using namespace animations;

#define DATA_PIN 14

#define BOARD_CURRENT_MA 250
#define MAX_DRAW_MA 2000

#define NUM_LEDS 256 * 2

CRGBArray<NUM_LEDS> leds;

void setup() {
  Serial.begin(9600);

  Serial.println("WiFi Setup...");
  wifi::setup();

  Serial.println("Home Assistant Setup...");
  home_assistant::setup();

  Serial.println("LED Setup...");
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_DRAW_MA - BOARD_CURRENT_MA);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();

  Serial.println("Setup complete.");
}

uint16_t to_led_index(const uint8_t x, const uint8_t y) {
  if (x % 2) {
    return (15 - y) + x * 16;
  } else {
    return y + x * 16;
  }
}

const unsigned int CHAR_PADDING = (32 / CHARS_WIDTH) + 1;

uint8_t animation_index = 0;
long local_t = 0;

void loop() {
  long t = millis();
  local_t += 1;

  Animation *a = get_animations()[animation_index];
  if (local_t > a->get_duration()) {
    local_t = 0;
    do {
      animation_index += 1;
      animation_index %= get_animations().size();
      a = get_animations()[animation_index];
    } while (!a->get_enabled());
  }

  FastLED.clear();
  for (uint8_t x = 0; x < 32; ++x) {
    for (uint8_t y = 0; y < 16; ++y) {
      uint16_t i = to_led_index(x, y);
      leds[i] = a->get_pixel(x, y, local_t);
    }
  }
  FastLED.show();

  home_assistant::loop();

  long length = millis() - t;
  if (length < 33) {
    delay(33 - length);
  }
}
