#ifndef Temperture_Sensor_h_
#define Temperture_Sensor_h_

#include <Arduino.h>

class Temparature_Senser {
  public:
    Temparature_Senser (uint8_t pins , int R1);
   // float R1 = 10000;
    float getTempinC ();
    void setSensitivity( float value);
    
  private:
    uint8_t pins;
    int R1;
    float error;
};

#endif
