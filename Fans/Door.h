#ifndef DOOR_H
#define DOOR_H

#include "HomeSpan.h"
#include "Config.h"

#define OPEN_PIN 15
#define DOOR_VOLTAGE_SENSOR 34
#define ADC_MAX_READ_VOLTAGE 3.3
#define ADC_MAX_ANALOG_VALUE 4095
#define GATE_CLOSE_DELAY_MS 13000

struct Door : Service::Door {
  private:
    SpanCharacteristic *CurrentPosition;
    SpanCharacteristic *TargetPosition;
    SpanCharacteristic *PositionState;

    unsigned long transitionEndTime;

  public:
    
    double readVoltage() {
      int adc_reading = analogRead(DOOR_VOLTAGE_SENSOR);
      Serial.println(String(adc_reading));
      double scalar = (double)adc_reading / ADC_MAX_ANALOG_VALUE ;
      return scalar * ADC_MAX_READ_VOLTAGE;
    }
    
    Door() : Service::Door()
    {
      CurrentPosition = new Characteristic::CurrentPosition();
      TargetPosition = new Characteristic::TargetPosition();
      PositionState = new Characteristic::PositionState();

      CurrentPosition->setRange(0, 1, 1);
      TargetPosition->setRange(0, 1, 1);

      pinMode(OPEN_PIN, OUTPUT);
      digitalWrite(OPEN_PIN, LOW);
      pinMode(DOOR_VOLTAGE_SENSOR, INPUT);
    }

    boolean update() {
      if (this->TargetPosition->isUpdated) {
        digitalWrite(OPEN_PIN, HIGH);
        delay(100);
        digitalWrite(OPEN_PIN, LOW);
        transitionEndTime = millis() + GATE_CLOSE_DELAY_MS;
      }

      return true;
    }

    void loop() {
      uint8_t currentPos = this->CurrentPosition->getVal();
      uint8_t targetPos = this->TargetPosition->getNewVal();

      double voltage = readVoltage();
      if (voltage <= 0.2) {
        // door is definitely closed
        this->CurrentPosition->setVal(0);
        this->TargetPosition->setVal(0);
      }

      else if (voltage >= 2.7 && voltage <= 3) {
        this->CurrentPosition->setVal(1);
        this->TargetPosition->setVal(1);
      }
    }
};

#endif
