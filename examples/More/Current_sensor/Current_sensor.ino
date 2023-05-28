#include <Current_sensor.h>
#include <Sampler.h>
Current_sensor Sensor (A1); // pin , R1 , OP_ AMP Gain

Sampler Sampler1 (100 , 10); 

void setup() {
  Serial.begin(9600);
  //
  //Sensor.calibrate(0.8155);
  Sensor.setSensitivity(4.105);
}

void loop() {

  Serial.print("Current");
  Serial.print(" : ");
  Serial.println(Sampler1.Sampled(Sensor.getCurrent()));

}
