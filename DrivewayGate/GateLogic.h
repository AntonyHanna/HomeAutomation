#include "HomeSpan.h"
#include "AverageArray.h"

#define OPEN_PIN 15
#define DOOR_VOLTAGE_SENSOR 34
#define ERROR_MARGIN 100
#define AVG_SAMPLE_SIZE 20
#define GATE_CLOSE_DELAY_MS 13000
#define SAMPLES_PER_SECOND 10

struct DrivewayGate : Service::Door {
  private:
    SpanCharacteristic *CurrentPosition;
    SpanCharacteristic *TargetPosition;
    SpanCharacteristic *PositionState;

    AverageArray *array;
    unsigned long transitionEndTime;
    unsigned long currentTime;

    enum class GateRef {
      Open    = 0,
      Transitioning = 300,
      Closed  = 670
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

      this->array = new AverageArray(AVG_SAMPLE_SIZE);
      this->transitionEndTime = 0;
      this->currentTime = millis();
    }

    boolean update() {
      // trigger the gate
      if (this->TargetPosition->updated()) {
        digitalWrite(OPEN_PIN, HIGH);
        delay(100);
        digitalWrite(OPEN_PIN, LOW);
        
        this->transitionEndTime = this->currentTime + GATE_CLOSE_DELAY_MS;
      }

      return true;
    }

    // might need a transition period where we look at voltage and wait 8 seconds then show state
    void loop() {
      this->currentTime = millis();

      static uint16_t previousVoltage;
      static PositionRefs lastFullPositionState;

      int avg = 0;
      uint16_t voltage = analogRead(DOOR_VOLTAGE_SENSOR);
      avg = this->array->Insert(voltage);

      Serial.print("Average: ");
      Serial.println(avg);

      PositionRefs currentPositionState = DetermineState(avg);

      // Determing opening and closing based on what the last primary state was
      // After transitional state
      // Set current state to what ever the current state is

      // Detect a voltage state change
      // --------|PREVIOUS - MARGIN|----|READING|----|PREVIOUS + MARGIN|-------- //
      if(!isWithinRange(voltage, previousVoltage, ERROR_MARGIN, ERROR_MARGIN) && !this->transitionEndTime) {
        Serial.print("This voltage: [");
        Serial.print(voltage);
        Serial.println("] is looking kinda SUS, entering transitioning state...");
        Serial.println(avg);

        this->transitionEndTime = this->currentTime + GATE_CLOSE_DELAY_MS;
      }

      if(this->transitionEndTime) {
        if(isWithinRange(avg, (uint16_t)GateRef::Transitioning, ERROR_MARGIN, ERROR_MARGIN)) {
          if(this->CurrentPosition->getVal() == (uint16_t)PositionRefs::Open) {
            this->TargetPosition->setVal((uint16_t)PositionRefs::Closed);
          } else {
            this->TargetPosition->setVal((uint16_t)PositionRefs::Open);
          }
        }
      }

      // Covers initial state of 0
      if(!this->transitionEndTime || currentTime >= this->transitionEndTime) {
        this->CurrentPosition->setVal((uint16_t)currentPositionState);
        this->TargetPosition->setVal((uint16_t)currentPositionState);

        lastFullPositionState = currentPositionState;
        this->transitionEndTime = 0;
      }

      previousVoltage = voltage;
      delay(1000 / SAMPLES_PER_SECOND);
    }

    PositionRefs DetermineState(uint16_t voltage) {
      PositionRefs state;

      if(isWithinRange(voltage, (uint16_t)GateRef::Open, ERROR_MARGIN, ERROR_MARGIN)) 
        state = PositionRefs::Open;

      else if(isWithinRange(voltage, (uint16_t)GateRef::Closed, ERROR_MARGIN, ERROR_MARGIN)) 
        state = PositionRefs::Closed;

      return state;
    }
    // --------|SECOND - MARGIN|----|FIRST|----|SECOND + MARGIN|-------- //
    // Is first value within range of second value
    bool isWithinRange(uint16_t first, uint16_t second, uint16_t upper_margin, uint16_t lower_margin) {
      return second - lower_margin < first && second + upper_margin > first;
    }
};
