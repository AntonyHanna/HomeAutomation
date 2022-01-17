#ifndef RF_RADIO_H
#define RF_RADIO_H

#include <RCSwitch.h>

#define RADIO_TX_PIN 2
#define TX_PROTOCOL 11
#define RADIO_PULSE_LENGTH 339
#define RADIO_TX_MESSAGE_BITS 12

struct RFRadio {
  RCSwitch rc_switch;

  RFRadio() {
    this->rc_switch = RCSwitch();

    this->rc_switch.enableTransmit(RADIO_TX_PIN);
    this->rc_switch.setProtocol(TX_PROTOCOL);
    this->rc_switch.setPulseLength(RADIO_PULSE_LENGTH);
  }

  void send(int val) {
    this->rc_switch.send(val, RADIO_TX_MESSAGE_BITS);
  }
};
#endif
