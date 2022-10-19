#ifndef GARAGEDOOROPENER_H
#define GARAGEDOOROPENER_H

#define INDUCTIVE_SENSOR_PIN 23
#define TRANSISTOR_PIN 22
#define GATE_CLOSE_DELAY_MS 11000
#define MILLIS_MAX_DIFFERENCE 20000

#include "Config.h"
#include "HomeSpan.h"

struct GarageDoorOpener : Service::GarageDoorOpener {
  private:
    // Minimum viable characteristics
    SpanCharacteristic *currentDoorState;
    SpanCharacteristic *targetDoorState;
    SpanCharacteristic *obstructionDetected;

    bool useGenericTracking;
    unsigned long targetTime;
    bool firstRun = true;

  public:
    GarageDoorOpener() : Service::GarageDoorOpener() {
      this->currentDoorState = new Characteristic::CurrentDoorState();
      this->targetDoorState = new Characteristic::TargetDoorState();
      this->obstructionDetected = new Characteristic::ObstructionDetected();

      this->useGenericTracking = true;

      // Setup the required pins
      pinMode(TRANSISTOR_PIN, OUTPUT);
      digitalWrite(TRANSISTOR_PIN, LOW);

      pinMode(INDUCTIVE_SENSOR_PIN, INPUT);
    }

    boolean update() {
      if (targetDoorState->isUpdated) {
        this->useGenericTracking = false;

        digitalWrite(TRANSISTOR_PIN, HIGH);
        delay(100);
        digitalWrite(TRANSISTOR_PIN, LOW);

        targetTime = millis() + GATE_CLOSE_DELAY_MS;
        Serial.println("Entering HomeKit transitional period");
      }

      return true;
    }

    void loop() {
      static bool previousGateState;
      bool currentGateState = !digitalRead(INDUCTIVE_SENSOR_PIN);
      unsigned long currentTime = millis();
      static unsigned long transitionEndTime;

      // If millis was to overflow we don't want to wait approx 50 days to update the state again
      // basically if we detect an abnormal distance between the time we're waiting for and the current time
      // then if there is a value set for either target times set it to a new valid end time
      if (abs(transitionEndTime - currentTime) > MILLIS_MAX_DIFFERENCE || abs(targetTime - currentTime) > MILLIS_MAX_DIFFERENCE) {
        if(transitionEndTime) {
          transitionEndTime = currentTime + GATE_CLOSE_DELAY_MS;
        }
        if(targetTime) {
          targetTime = currentTime + GATE_CLOSE_DELAY_MS;
        }
      }

      if (this->useGenericTracking) {
        // Sensor based tracking
        if (firstRun) {
          firstRun = false;
          transitionEndTime = currentTime + 10000; // give a 10 sec delay to boot and connect to internet then check initial state
        }

        if (transitionEndTime == 0 && previousGateState != currentGateState) {
          Serial.println("Entering transitional period");
          transitionEndTime = currentTime + GATE_CLOSE_DELAY_MS;
        }

        // run when no transitional period is active
        // when transitional period is active, don't run until it's done
        if (transitionEndTime == 0 || (transitionEndTime != 0 && currentTime >= transitionEndTime)) {
          this->currentDoorState->setVal(currentGateState);
          this->targetDoorState->setVal(currentGateState);

          if (transitionEndTime != 0 && currentTime >= transitionEndTime) {
            Serial.println("Transitional preiod finished, state has been updated");
            transitionEndTime = 0;
          }
        }
      }
      else {
        // HomeKit tracking logic
        if (this->targetTime != 0 && currentTime >= this->targetTime) {
          this->currentDoorState->setVal(currentGateState);
          this->targetTime = 0;
          this->useGenericTracking = true;
        }
      }

      previousGateState = currentGateState;
    }
};

#endif
