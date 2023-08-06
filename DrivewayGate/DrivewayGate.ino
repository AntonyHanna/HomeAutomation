#include "HomeSpan.h"
#include "GateLogic.h"

void PrepareAccessory() {
  new SpanAccessory();

  new Service::AccessoryInformation();
  new Characteristic::Name("Gate Interface");
  new Characteristic::Manufacturer("Antony");
  new Characteristic::SerialNumber("ESP32");
  new Characteristic::Model("ESP32-WROOM");
  new Characteristic::FirmwareRevision("1.0");
  new Characteristic::Identify();

  new Service::HAPProtocolInformation();
  new Characteristic::Version("1.1.0");
}

void setup() {
  Serial.begin(115200);
  WiFi.setSleep(false);
  homeSpan.enableOTA();
  homeSpan.begin(Category::Doors, "Driveway Gate", "DrivewayGate");
  PrepareAccessory();

  new DrivewayGate();
}

void loop() {
  homeSpan.poll();
}
