#pragma once

#include "./animations.h"

#include <ArduinoHA.h>
#include <vector>

namespace home_assistant {
void setup();

void loop();

void update_interface();

struct Light {
  HALight *ha;
  animations::Animation *a;
};

} // namespace home_assistant
