#include <RCSwitch.h>
#include "HomeSpan.h"
#include "GenericFan.cpp"

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::Fans, "Ceiling Fan");

  GenericFan::begin();

  mySwitch.enableTransmit(2);

  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(11);

  // Optional set pulse length.
  mySwitch.setPulseLength(332);

  new GenericFan(mySwitch);
}

void loop() {
    homeSpan.poll();
}
