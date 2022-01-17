#include "HomeSpan.h"
#include "GenericFan.h"
#include "RFRadio.h"

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::Fans, "Ceiling Fan");
  RFRadio *rf = new RFRadio();

  GenericFan::begin();
  new GenericFan(rf);
}

void loop() {
  homeSpan.poll();
}
