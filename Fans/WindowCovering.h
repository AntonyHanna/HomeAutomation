#ifndef WINDOWCOVERING_H
#define WINDOWCOVERING_H

#include "Config.h"
#include "HomeSpan.h"

#define UP_PIN 21
#define STOP_PIN 19
#define DOWN_PIN 18

struct WindowCovering : Service::WindowCovering {
  private:
    // Minimum viable characteristics
    SpanCharacteristic *targetPosition;
    SpanCharacteristic *currentPosition;
    SpanCharacteristic *positionState;

  public:
    WindowCovering() : Service::WindowCovering() {
      this->targetPosition = new Characteristic::TargetPosition();
      this->currentPosition = new Characteristic::CurrentPosition();
      this->targetPosition->setRange(0, 100, 10);
      this->positionState = new Characteristic::PositionState();

      pinMode(UP_PIN, OUTPUT);
      pinMode(STOP_PIN, OUTPUT);
      pinMode(DOWN_PIN, OUTPUT);

      digitalWrite(UP_PIN, LOW);
      digitalWrite(STOP_PIN, LOW);
      digitalWrite(DOWN_PIN, LOW);
    }

    boolean update() {
      uint8_t oldTargetVal = targetPosition->getVal();
      uint8_t newTargetVal = targetPosition->getNewVal();

      // needs additgional logic fr showing the transitioning states
      // do the above when we have a sensor solution

      if(oldTargetVal > newTargetVal) {
        Serial.println("Closing the door");
        digitalWrite(DOWN_PIN, HIGH);
        delay(100);
        digitalWrite(DOWN_PIN, LOW);
        
        this->currentPosition->setVal(0);
      }

      else if(newTargetVal > oldTargetVal) {
        Serial.println("Opening the door");
        digitalWrite(UP_PIN, HIGH);
        delay(100);
        digitalWrite(UP_PIN, LOW); 
        
        this->currentPosition->setVal(100);
      }

      else {
        Serial.println("Value should be the same do nothing");
      }
      
      return true;
    }
};

#endif
