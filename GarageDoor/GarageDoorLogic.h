#ifndef GARAGE_DOOR_LOGIC_H
#define GARAGE_DOOR_LOGIC_H

#include "HomeSpan.h"

#define INDUCTIVE_SENSOR_PIN 23
#define TRANSISTOR_PIN 22
#define GATE_CLOSE_DELAY_MS 11000
#define MILLIS_MAX_DIFFERENCE 20000

class GarageDoorLogic : Service::GarageDoorOpener {
private:
  SpanCharacteristic *currentDoorState;
  SpanCharacteristic *targetDoorState;
  SpanCharacteristic *obstructionDetected;

  long long transitionEndTime;
  unsigned long currentTime;

  enum class CurrentDoorStateRef {
    Open = 0,
    Closed = 1,
    Opening = 2,
    Closing = 3,
    Stopped = 4
  };

  enum class TargetDoorStateRef {
    Open = 0,
    Closed = 1
  };


public:
  GarageDoorLogic() : Service::GarageDoorOpener() {
    this->currentDoorState = new Characteristic::CurrentDoorState();
    this->targetDoorState = new Characteristic::TargetDoorState();
    this->obstructionDetected = new Characteristic::ObstructionDetected();

    // Setup the required pins
    pinMode(TRANSISTOR_PIN, OUTPUT);
    digitalWrite(TRANSISTOR_PIN, LOW);

    pinMode(INDUCTIVE_SENSOR_PIN, INPUT);

    this->transitionEndTime = 0;
    this->currentTime = millis();
  }

  boolean update() {
    if (targetDoorState->updated()) {
      digitalWrite(TRANSISTOR_PIN, HIGH);
      delay(100);
      digitalWrite(TRANSISTOR_PIN, LOW);

      this->transitionEndTime = this->currentTime + GATE_CLOSE_DELAY_MS;
    }

    return true;
  }

  void loop() { 
    this->currentTime = millis();

    static bool lastDoorState;
    static bool transitionStateSet;

   
    bool currentDoorState = !digitalRead(INDUCTIVE_SENSOR_PIN);

    if (lastDoorState != currentDoorState && !this->transitionEndTime)
      this->transitionEndTime = this->currentTime + GATE_CLOSE_DELAY_MS;

    if (this->transitionEndTime > this->currentTime) {
      // stops us from constantly updating target state while already showing a transition state
      if(!transitionStateSet) {
        uint8_t transitionState = getTransitionalState(this->currentDoorState->getVal());
        this->targetDoorState->setVal(transitionState);

        transitionStateSet = true;
      }
      
    } else {
      this->currentDoorState->setVal(currentDoorState);
      this->targetDoorState->setVal(currentDoorState);
      this->transitionEndTime = 0;

      transitionStateSet = false;
    }

    lastDoorState = currentDoorState;
  }

  uint8_t getTransitionalState(bool doorState) {
    if (doorState == (uint8_t)CurrentDoorStateRef::Open)
      return (uint8_t)TargetDoorStateRef::Closed;

    if (doorState == (uint8_t)CurrentDoorStateRef::Closed)
      return (uint8_t)TargetDoorStateRef::Open;
  }
};
#endif