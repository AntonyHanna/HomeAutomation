#include "HomeSpan.h"
#include "AverageArray.h"

#define OPEN_PIN 15
#define DOOR_VOLTAGE_SENSOR 34
#define ERROR_MARGIN 100
#define SAMPLE_RANGE 10
#define GATE_CLOSE_DELAY_MS 13000

struct DrivewayGate : Service::Door {
  private:
    SpanCharacteristic *CurrentPosition;
    SpanCharacteristic *TargetPosition;
    SpanCharacteristic *PositionState;

    // Outlines the various Vrefs provided by the gate during the various movement states
    enum class GateRef {
      Open    = 800,
      Opening = 400,
      Closing = 266,
      Closed  = 0
    };

    enum class PositionRefs {
      Closed = 0,
      Open = 1
    };


  public:
    DrivewayGate() : Service::Door()
    {
      CurrentPosition = new Characteristic::CurrentPosition();
      TargetPosition = new Characteristic::TargetPosition();
      PositionState = new Characteristic::PositionState();

      CurrentPosition->setRange(0, 1, 1);
      TargetPosition->setRange(0, 1, 1);

      pinMode(OPEN_PIN, OUTPUT);
      pinMode(DOOR_VOLTAGE_SENSOR, INPUT);
      digitalWrite(OPEN_PIN, LOW);
    }

    boolean update() {
      // trigger the gate
      if (this->TargetPosition->updated()) {
        digitalWrite(OPEN_PIN, HIGH);
        delay(100);
        digitalWrite(OPEN_PIN, LOW);
      }

      // Will update target value
      Serial.println(this->TargetPosition->getVal());
      return true;
    }

    // might need a transition period where we look at voltage and wait 8 seconds then show state
    void loop() {
      static AverageArray *array = NULL;
      static unsigned long transitionEndTime;
      static uint16_t previousVoltage;
      static PositionRefs lastFullPositionState;

      unsigned long currentTime = millis();
      uint16_t voltage = analogRead(DOOR_VOLTAGE_SENSOR);
      int avg = 0;

      PositionRefs currentPositionState = DetermineState(voltage);

      if(array == NULL) {
        array = new AverageArray(SAMPLE_RANGE);
      }

      avg = array->Insert(voltage);

      // Determing opening and closing based on what the last primary state was
      // After transitional state
      // Set current state to what ever the current state is

      // Detect a voltage state change
      // --------|PREVIOUS - MARGIN|----|READING|----|PREVIOUS + MARGIN|-------- //
      if(!isWithinRange(voltage, previousVoltage, ERROR_MARGIN, ERROR_MARGIN)) {
        Serial.print("This voltage: [");
        Serial.print(voltage);
        Serial.println("] is looking kinda SUS, entering transitioning state...");
        Serial.println(avg);

        transitionEndTime = currentTime + GATE_CLOSE_DELAY_MS;
      }

      if(transitionEndTime) {
        if(isWithinRange(avg, (uint16_t)GateRef::Opening, ERROR_MARGIN, ERROR_MARGIN)) {
          this->TargetPosition->setVal((uint16_t)PositionRefs::Open);
        }
        else if (isWithinRange(avg, (uint16_t)GateRef::Closing, ERROR_MARGIN, ERROR_MARGIN)) {
          this->TargetPosition->setVal((uint16_t)PositionRefs::Closed);
        }
      }

      // Covers initial state of 0
      if(!transitionEndTime || currentTime >= transitionEndTime) {
        this->CurrentPosition->setVal((uint16_t)currentPositionState);
        this->TargetPosition->setVal((uint16_t)currentPositionState);

        lastFullPositionState = currentPositionState;
        transitionEndTime = 0;
      }

      previousVoltage = voltage;
      delay(500);
    }
    // prod values:
    // 2.1 open
    // (1.05**) higher average closing
    // (0.7**) lower average opening
    // closed 0

    PositionRefs DetermineState(uint16_t voltage) {
      PositionRefs state;

      if(voltage >= (uint16_t)GateRef::Open)
        state = PositionRefs::Open;

      else if (voltage <= (uint16_t)GateRef::Closed + 50)
        state = PositionRefs::Closed;

      return state;
    }
    // --------|SECOND - MARGIN|----|FIRST|----|SECOND + MARGIN|-------- //
    // Is first value within range of second value
    bool isWithinRange(uint16_t first, uint16_t second, uint16_t upper_margin, uint16_t lower_margin) {
      Serial.print("--------[");
      Serial.print(second - lower_margin);
      Serial.print("]----[");
      Serial.print(first);
      Serial.print("]----[");
      Serial.print(second + upper_margin);
      Serial.println("]--------");

      return second - lower_margin < first && second + upper_margin > first;
    }
};
