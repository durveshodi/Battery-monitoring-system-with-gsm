#include <Voltage_sensor.h>
#include <Sampler.h>

Voltage_Sensor Sensor (A1 , 10000 , 1000); // pin , R1 ,R2
Sampler Sampler1 (1000 , 100); // 1 Seconds == 100 , Sample Number 10 so 100 Sample per Seconds

void setup() {
  Serial.begin(9600);
  Sensor.setSensitivity(0.8155);
}

void loop() {

  Serial.print("Voltage: ");
  Serial.print(" : ");
  Serial.println(Sampler1.Sampled(Sensor.getVoltage()));
  
}