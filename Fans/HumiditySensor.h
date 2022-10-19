#ifdef HUMIDITY_ENABLED
#ifndef HUMIDITY_SENSOR_H
#define HUMIDITY_SENSOR_H

#include "Config.h"
#include "HomeSpan.h"
#include "DHT.h"

struct HumiditySensor : Service::HumiditySensor {
  private:
    DHT dht = DHT(DHTPIN, DHTTYPE);
    SpanCharacteristic *currentHumidity;

  public:
    HumiditySensor() : Service::HumiditySensor() {
      dht.begin();
      this->currentHumidity = new Characteristic::CurrentRelativeHumidity();
    }

    void loop() {
      static unsigned long timestamp;
      if (millis() - timestamp >= SENSOR_POLL_RATE) {
        // get humidity from sensor
        float humidity = dht.readHumidity();
        this->currentHumidity->setVal(humidity);
        timestamp = millis();
      }
    }
};

#endif
#endif
