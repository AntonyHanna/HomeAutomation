#include "HomeSpan.h"
#include "GenericFan.h"
#include "RFRadio.h"

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::Fans, "Ceiling Fan");
  PrepareAccessory();
  
  RFRadio *rf = new RFRadio();
  new GenericFan(rf);
}

void loop() {
  homeSpan.poll();
}

void PrepareAccessory() {
  new SpanAccessory();

  new Service::AccessoryInformation();
  new Characteristic::Name("Ceiling Fan");
  new Characteristic::Manufacturer("Generic");
  new Characteristic::SerialNumber("ESP32_GenericFan");
  new Characteristic::Model("Fan");
  new Characteristic::FirmwareRevision("1.0");
  new Characteristic::Identify();

  new Service::HAPProtocolInformation();
  new Characteristic::Version("1.1.0");
}
