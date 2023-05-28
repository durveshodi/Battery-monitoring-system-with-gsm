#include "Current_sensor.h"

Current_sensor::Current_sensor (uint8_t _pins) {
  pins =  _pins;
  sen = 1;
  zero = 0;
}

void Current_sensor::setSensitivity( float value) {
  sen = value;
}
float Current_sensor::getCurrent () {
 int timer = 0;
  int16_t acc = 0;
  for (int i = 0; i < 10; i++) {
    acc += (analogRead(pins) - zero);
    
  }
  float  SensorValue = (float)acc/ 10  * (5.0 / 1023) * sen;
  return SensorValue;
}

int Current_sensor::calibrate() {
  uint16_t acc = 0;
  for (int i = 0; i < 10; i++) {
    acc += analogRead(pins);
  }
  zero = acc / 10;
  return zero;
}
