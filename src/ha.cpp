#include "ha.h"
#include "animations.h"
#include "config.h"

#include <ArduinoHA.h>
#include <WiFi.h>

namespace home_assistant {

HADevice device;
WiFiClient client;
HAMqtt mqtt(client, device, 2);

std::vector<Light> lights{};

void update_interface() {
  Serial.println("Update interface");
  for (Light l : lights) {
    l.ha->setState(l.a->get_enabled());
  }
}

void on_state_command(bool state, HALight *sender) {
  for (Light l : lights) {
    if (l.ha == sender) {
      l.a->set_enabled(state);
    }
  }
  update_interface();
}

void setup() {
  byte mac[6];
  WiFi.macAddress(mac);
  device.setUniqueId(mac, sizeof(mac));

  Serial.print("MAC & Device ID: ");
  for (uint8_t i = 0; i < 6; ++i) {
    char hexCar[2];

    sprintf(hexCar, "%02X", mac[i]);
    Serial.print(hexCar);
  }
  Serial.println();

  device.setName("LED Panel");
  device.setManufacturer("https://github.com/wmmiii/");
  device.setSoftwareVersion("0.1.9");

  for (animations::Animation* a : animations::get_animations()) {
    String id = a->get_name();
    id.toLowerCase();
    id.replace(" ", "_");
    HALight *ha = new HALight(id.c_str());
    ha->setName(a->get_name().c_str());
    ha->onStateCommand(on_state_command);
    ha->setIcon("mdi:lightbulb");
    lights.push_back({
        .ha = ha,
        .a = a,
    });
  }

  mqtt.begin(BROKER_ADDR, BROKER_PORT, BROKER_USERNAME, BROKER_PASSWORD);

  update_interface();
}

void loop() { mqtt.loop(); }

} // namespace home_assistant
