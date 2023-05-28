#include "Voltage_sensor.h"

Voltage_Sensor::Voltage_Sensor (uint8_t _pins , int _R1 , int _R2) {
  R1 = _R1;
  R2 = _R2;
  pins =  _pins;
  sen = 1;
}

void Voltage_Sensor::setSensitivity( float value) {
  sen = value;
}
float Voltage_Sensor::getVoltage () {

  int16_t acc = 0;
  for (int i = 0; i < 10; i++) {
    acc += analogRead(pins);
  }

  float  SensorValue = ((float)acc / 10 * 5.0 / 1024 * sen * (R1 + R2) / R2) ;

  return SensorValue;
}

int Voltage_Sensor::calibrate() {
 uint16_t acc = 0;
  for (int i = 0; i < 10; i++) {
    acc += analogRead(pins);
  }
  zero = acc / 10;
  return zero;
}
