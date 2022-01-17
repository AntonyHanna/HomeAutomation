#include <RCSwitch.h>
#include "HomeSpan.h"

struct GenericFan : Service::Fan {
  RCSwitch rcSwitch;

  SpanCharacteristic *active;
  SpanCharacteristic *rotationDirection;
  SpanCharacteristic *rotationSpeed;

  GenericFan(RCSwitch _rcSwitch) : Service::Fan()
  {
    
    this->rcSwitch = _rcSwitch;

    active = new Characteristic::Active();
    rotationSpeed = new Characteristic::RotationSpeed(1);
    rotationSpeed->setRange(0, 3, 1);
  }

  static void begin()
  {
    new SpanAccessory();

    new Service::AccessoryInformation();
    new Characteristic::Name("Ceiling Fan");
    new Characteristic::Manufacturer("Generic");
    new Characteristic::SerialNumber("ESP32_GenericFan");
    new Characteristic::Model("Fan");
    new Characteristic::FirmwareRevision("1.0");
    new Characteristic::Identify();

    new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");
  }

  boolean update()
  {
    if (active->isUpdated || rotationSpeed->isUpdated)
    {
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

      switch (_speed)
      {
        case 0:
          this->rcSwitch.send(1149, 12);
          break;

        case 1:
          this->rcSwitch.send(1143, 12);
          break;

        case 2:
          this->rcSwitch.send(1135, 12);
          break;

        case 3:
          this->rcSwitch.send(1119, 12);
          break;
      }
    }

    //    else if (rotationDirection->isUpdated)
    //    {
    //      this->rcSwitch.send(2196494, 24);
    //    }

    return true;
  }
};
