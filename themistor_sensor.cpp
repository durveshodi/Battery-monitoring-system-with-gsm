#include "themistor_sensor.h"

Temparature_Senser::Temparature_Senser (uint8_t _pins , int _R1) {
  R1 = _R1;
  pins =  _pins;
  error = 1;
}

void Temparature_Senser::setSensitivity( float value) {
  error = value;
}
float Temparature_Senser::getTempinC () {

  float logR2, R2, T, Tc, Tf;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  int16_t acc = 0;
  for (int i = 0; i < 10; i++) {
    acc += analogRead(pins);
  }
  R2 = R1 * (1023.0 / ((float)acc / 10) - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  Tc = T - 273.15;
  return Tc * error;
}
