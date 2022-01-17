#ifndef GENERIC_FAN_H
#define GENERIC_FAN_H

#include "RFRadio.h"
#include "HomeSpan.h"
#include "Config.h"

struct GenericFan : Service::Fan {
  private:
    RFRadio *radio;
    int fan_speeds;
    int change_rotation;
    int *speed_mappings;

    SpanCharacteristic *active;
    SpanCharacteristic *rotationDirection;
    SpanCharacteristic *rotationSpeed;

  public:
    // speeds count should include off
    GenericFan(int speeds, int *speeds_map) : Service::Fan()
    {
      this->radio = new RFRadio();
      this->fan_speeds = speeds;
      this->speed_mappings = speeds_map;

      active = new Characteristic::Active();
      rotationSpeed = new Characteristic::RotationSpeed(1);
      rotationSpeed->setRange(0, fan_speeds - 1, 1);
    }

    boolean update() {
      if (active->isUpdated || rotationSpeed->isUpdated) {
        int _speed = active->getNewVal() * rotationSpeed->getNewVal();
        int msg = this->speed_mappings[_speed];
        this->radio->send(msg);
      }

#ifdef CHANGE_ROTATION
      else if (rotationDirection->isUpdated)
      {
        this->radio->send(rotation_signal);
      }
#endif

      return true;
    }
};

#endif
