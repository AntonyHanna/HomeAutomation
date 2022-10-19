#include "HomeSpan.h"
#include "GenericFan.h"
#include "Config.h"

#ifdef TEMPERATURE_ENABLED
#include "TemperatureSensor.h"
#endif

#ifdef HUMIDITY_ENABLED
#include "HumiditySensor.h"
#endif

#ifdef DOOR_ENABLED
#include "Door.h"
#endif

#ifdef GARAGEDOOROPENER_ENABLED
#include "GarageDoorOpener.h"
#endif

#ifdef WINDOWCOVERING_ENABLED
#include "WindowCovering.h"
#endif

void PrepareAccessory();

void setup() {
  Serial.begin(115200);
  WiFi.setSleep(false);

  homeSpan.begin(Category::WindowCoverings, "Sliding Gate");
  PrepareAccessory();

#ifdef DOOR_ENABLED
  new Door();
#endif

#ifdef FAN_ENABLED

  int *speed_mappings = new int[TOTAL_FAN_SPEEDS] {
    SPEED_MAPPINGS
  };

  new GenericFan(TOTAL_FAN_SPEEDS, speed_mappings);
#endif

#ifdef GARAGEDOOROPENER_ENABLED
  new GarageDoorOpener();
#endif

#ifdef WINDOWCOVERING_ENABLED
  new WindowCovering();
#endif

#ifdef TEMPERATURE_ENABLED
  new TemperatureSensor();
#endif

#ifdef HUMIDITY_ENABLED
  new HumiditySensor();
#endif
}

void loop() {
  homeSpan.poll();
}

void PrepareAccessory() {
  new SpanAccessory();

  new Service::AccessoryInformation();
  new Characteristic::Name("ESP32_HomeKit_Device");
  new Characteristic::Manufacturer("Antony");
  new Characteristic::SerialNumber("ESP32");
  new Characteristic::Model("ESP32-WROOM");
  new Characteristic::FirmwareRevision("1.0");
  new Characteristic::Identify();

  new Service::HAPProtocolInformation();
  new Characteristic::Version("1.1.0");
}
