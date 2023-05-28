#ifndef Voltage_sensor_h_
#define Voltage_sensor_h_

#include <Arduino.h>

class Voltage_Sensor {
  public:
    Voltage_Sensor (uint8_t pins , int R1 , int R2);
    float getVoltage ();
    int calibrate();
    void setSensitivity( float value);
    
  private:
    uint8_t pins;
    int R1;
    int R2;
    float sen;
    int zero;
};

#endif
