#ifndef CONFIG_H
#define CONFIG_H

// Remote config
#define TOTAL_FAN_SPEEDS 4
#define SPEED_MAPPINGS\ /* the indexes represent the various speeds */
    1149,\ 
    1143,\
    1135,\
    1119

// Fan Accesory config
//#define CHANGE_ROTATION 1

// RF Radio config
#define RADIO_TX_PIN 2
#define TX_PROTOCOL 11
#define RADIO_PULSE_LENGTH 339
#define RADIO_TX_MESSAGE_BITS 12

// Garage Door Opener Config
//#define GARAGEDOOROPENER_ENABLED 1
#ifdef GARAGEDOOROPENER_ENABLED
#endif

#endif
