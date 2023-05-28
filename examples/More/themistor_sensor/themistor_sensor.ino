#include <themistor_sensor.h>

Temparature_Senser Senser (A1 , 9740);
Temparature_Senser Senser2 (A2 ,9650);

void setup() {
  Serial.begin(9600);
  Senser.SetERRORValue(0.845488);
}

void loop() {

  Serial.print("Temperature: ");
  Serial.print(" C1");
  Serial.print(" : ");
  Serial.print(Senser.getTempinC());
  Serial.print(", C2");
  Serial.print(" : ");
  Serial.print(Senser2.getTempinC());
  Serial.println("");
  
  delay(500);
}
