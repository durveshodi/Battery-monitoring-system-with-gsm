#ifndef Current_sensor_h_
#define Current_sensor_h_

#include <Arduino.h>

class Current_sensor {
  public:
    Current_sensor (uint8_t pins);
    float getCurrent ();
    int calibrate();
    void setSensitivity( float value);
    
  private:
    uint8_t pins;
    float sen;
    int zero;
};

#endif
