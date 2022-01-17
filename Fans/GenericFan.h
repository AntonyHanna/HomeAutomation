#ifndef GENERIC_FAN_H
#define GENERIC_FAN_H

#include "RFRadio.h"
#include "HomeSpan.h"

//#define CHANGE_ROTATION 1

struct GenericFan : Service::Fan {
  RFRadio radio;

  SpanCharacteristic *active;
  SpanCharacteristic *rotationDirection;
  SpanCharacteristic *rotationSpeed;

  GenericFan(RFRadio *_radio) : Service::Fan()
  {
    this->radio = *_radio;

    active = new Characteristic::Active();
    rotationSpeed = new Characteristic::RotationSpeed(1);
    rotationSpeed->setRange(0, 3, 1);
  }

  boolean update() {
    if (active->isUpdated || rotationSpeed->isUpdated) {
      int _speed = active->getNewVal() * rotationSpeed->getNewVal();
      //      switch (_speed)
      //      {
      //        case 0:
      //          this->rcSwitch.send(2196481, 24);
      //          break;
      //
      //        case 1:
      //          this->rcSwitch.send(2196480, 24);
      //          break;
      //
      //        case 2:
      //          this->rcSwitch.send(2196501, 24);
      //          break;
      //
      //        case 3:
      //          this->rcSwitch.send(2196493, 24);
      //          break;
      //
      //        case 4:
      //          this->rcSwitch.send(2196489, 24);
      //          break;
      //
      //        case 5:
      //          this->rcSwitch.send(2196490, 24);
      //          break;
      //      }

      switch (_speed) {
        case 0:
          this->radio.send(1149);
          break;

        case 1:
          this->radio.send(1143);
          break;

        case 2:
          this->radio.send(1135);
          break;

        case 3:
          this->radio.send(1119);
          break;
      }
    }
#ifdef CHANGE_ROTATION
        else if (rotationDirection->isUpdated)
        {
          this->rcSwitch.send(2196494, 24);
        }
#endif
    return true;
  }
};

#endif
