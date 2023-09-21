#include "./chars.h"
#include <FastLED.h>

#define DATA_PIN 14

#define BOARD_CURRENT_MA 250
#define MAX_DRAW_MA 2000

#define NUM_LEDS 256 * 2

CRGBArray<NUM_LEDS> leds;

void setup() {
  Serial.begin(9600);
  Serial.println("LED Setup...");

  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_DRAW_MA - BOARD_CURRENT_MA);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();

  Serial.println("Setup complete.");
}

uint16_t to_led_index(uint8_t x, uint8_t y) {
  if (x % 2) {
    return (15 - y) + x * 16;
  } else {
    return y + x * 16;
  }
}

uint16_t x_pan;
String str = "PARTY TIME";
uint8_t h = 0;

const unsigned int CHAR_PADDING = (32 / CHARS_WIDTH) + 1;

void loop() {
  long start = millis();
  FastLED.clear();
  for (uint8_t x = 0; x < 32; ++x) {
    unsigned int char_index = (x + x_pan) / CHARS_WIDTH;
    if (char_index < CHAR_PADDING ||
        char_index - CHAR_PADDING >= str.length()) {
      continue;
    }
    char c = str.charAt(char_index - CHAR_PADDING);

    for (uint8_t y = 0; y < 16; ++y) {
      uint16_t i = to_led_index(x, y);
      uint8_t v_x = (x + x_pan) % CHARS_WIDTH;
      uint8_t v = CHARS[static_cast<unsigned int>(c)][v_x][y];
      leds[i] = CHSV(h, 255, v >> 1);
    }
  }
  FastLED.show();
  if (Serial.available() > 0) {
    str = Serial.readString();
    x_pan = 0;
  }
  long length = millis() - start;
  if (length < 33) {
    delay(33 - length);
  }

  x_pan++;
  if ((x_pan + 32) / CHARS_WIDTH >= str.length() + CHAR_PADDING * 2) {
    x_pan = 0;
  }
  h++;
}
