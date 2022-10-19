#ifdef TEMPERATURE_ENABLED
#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "Config.h"
#include "HomeSpan.h"
#include "DHT.h"

struct TemperatureSensor : Service::TemperatureSensor {
  private:
    DHT dht = DHT(DHTPIN, DHTTYPE);
    SpanCharacteristic *currentTemperature;

  public:
    TemperatureSensor() : Service::TemperatureSensor() {
      dht.begin();
      this->currentTemperature = new Characteristic::CurrentTemperature();
    }

    void loop() {
      static unsigned long timestamp;
      if (millis() - timestamp >= SENSOR_POLL_RATE) {
        // get temperature from sensor
        float temp = dht.readTemperature();
        this->currentTemperature->setVal(temp);
        timestamp = millis();
      }
    }
};

#endif
#endif
