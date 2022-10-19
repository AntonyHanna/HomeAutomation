#ifndef CONFIG_H
#define CONFIG_H

// Remote config
// Fan Accesory config
//#define FAN_ENABLED 1
#ifdef FAN_ENABLED

#define TOTAL_FAN_SPEEDS 6
#define SPEED_MAPPINGS \
  2196481,\
  2196480,\
  2196501,\
  2196493,\
  2196489,\
  2196490



//#define CHANGE_ROTATION 1
#ifdef CHANGE_ROTATION
#define CHANGE_ROTATION_MSG 2196494
#endif
#endif

// RF Radio config
#define RADIO_TX_PIN 2
#define TX_PROTOCOL 1
#define RADIO_PULSE_LENGTH 256
#define RADIO_TX_MESSAGE_BITS 24

// Temperature & Humidity config
//#define TEMPERATURE_ENABLED 1
//#define HUMIDITY_ENABLED 1

#ifdef TEMPERATURE_ENABLED || HUMIDITY_ENABLED
#define DHTPIN 4
#define DHTTYPE DHT11
#define SENSOR_POLL_RATE 2100 /* requires at least 1000ms per poll*/
#endif

#define DOOR_ENABLED 1
#ifdef DOOR_ENABLED
#endif


// Garage Door Opener Config
//#define GARAGEDOOROPENER_ENABLED 1
#ifdef GARAGEDOOROPENER_ENABLED
#endif

//#define WINDOWCOVERING_ENABLED 1
#ifdef WINDOWCOVERING_ENABLED
#endif

// root endif
#endif
