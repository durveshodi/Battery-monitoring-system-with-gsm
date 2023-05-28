//-----------------   PWM D9 Pin   -----------------------

#ifndef pwm_H_
#define pwm_H_

/*---------------------*/
#define CurrentLimter1 9
#define CurrentLimter2 3
/*------------ -*/


/*---------------------------------*/
#include <Current_sensor.h>
Current_sensor CSensor1 (A6); // pin
Current_sensor CSensor2 (A2); // pin


void PWMFrequncy() {
  TCCR1B = TCCR1B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000010; // for PWM frequency of 3921.16 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000011; // for PWM frequency of 490.20 Hz (The DEFAULT)
  //TCCR1B = TCCR1B & B11111000 | B00000100; // for PWM frequency of 122.55 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000101; // for PWM frequency of 30.64 Hz
  //------------------------------------------------------------------------------------------

  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000010; // for PWM frequency of 3921.16 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000011; // for PWM frequency of 980.39 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000100; // for PWM frequency of 490.20 Hz (The DEFAULT)
  //TCCR2B = TCCR2B & B11111000 | B00000101; // for PWM frequency of 245.10 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

}

int PWM1value;
int PWM2value;

int PWM1MAXvalue;
int PWM2MAXvalue;
int PWM1MINvalue;
int PWM2MINvalue;

void setALLON() {
  analogWrite(CurrentLimter1 , 255);
  analogWrite(CurrentLimter2, 255);
}

void setCurrent(float value) {
  int NewValue = value * 10;
  switch (NewValue) {
    case 1:
      PWM1MAXvalue = 5;
      PWM2MAXvalue = 5;
      PWM2MINvalue = 1;
      PWM2MINvalue = 1;
      break;
    case 2:
      PWM1MAXvalue = 10;
      PWM2MAXvalue = 10;
      PWM2MINvalue = 5;
      PWM2MINvalue = 5;
      break;
    case 3:
      PWM1MAXvalue = 15;
      PWM2MAXvalue = 15;
      PWM2MINvalue = 10;
      PWM2MINvalue = 10;
      break;
    case 4:
      PWM1MAXvalue = 35;
      PWM2MAXvalue = 35;
      PWM2MINvalue = 10;
      PWM2MINvalue = 10;
      break;
    case 5:
      PWM1MAXvalue = 50;
      PWM2MAXvalue = 50;
      PWM2MINvalue = 35;
      PWM2MINvalue = 35;
      break;
    case 6:
      PWM1MAXvalue = 70;
      PWM2MAXvalue = 70;
      PWM2MINvalue = 51;
      PWM2MINvalue = 51;
      break;
    case  7:
      PWM1MAXvalue = 90;
      PWM2MAXvalue = 90;
      PWM2MINvalue = 71;
      PWM2MINvalue = 71;
      break;
    case 8:
      PWM1MAXvalue = 110;
      PWM2MAXvalue = 110;
      PWM2MINvalue = 91;
      PWM2MINvalue = 91;
      break;
    case 9:
      PWM1MAXvalue = 125;
      PWM2MAXvalue = 125;
      PWM2MINvalue = 111;
      PWM2MINvalue = 111;
      break;
    case 10:
      PWM1MAXvalue = 140;
      PWM2MAXvalue = 140;
      PWM2MINvalue = 126;
      PWM2MINvalue = 126;
      break;
    case 11:
      PWM1MAXvalue = 150;
      PWM2MAXvalue = 150;
      PWM2MINvalue = 141;
      PWM2MINvalue = 141;
      break;
    case 12:
      PWM1MAXvalue = 160;
      PWM2MAXvalue = 160;
      PWM2MINvalue = 151;
      PWM2MINvalue = 151;
      break;
    case 13:
      PWM1MAXvalue = 180;
      PWM2MAXvalue = 180;
      PWM2MINvalue = 161;
      PWM2MINvalue = 161;
      break;
    case 14:
      PWM1MAXvalue = 190;
      PWM2MAXvalue = 190;
      PWM2MINvalue = 181;
      PWM2MINvalue = 181;
      break;
    case 15:
      PWM1MAXvalue = 195;
      PWM2MAXvalue = 195;
      PWM2MINvalue = 191;
      PWM2MINvalue = 191;
      break;
    case 16:
      PWM1MAXvalue = 210;
      PWM2MAXvalue = 210;
      PWM2MINvalue = 200;
      PWM2MINvalue = 200;
      break;
    case 17:
      PWM1MAXvalue = 222;
      PWM2MAXvalue = 222;
      PWM2MINvalue = 211;
      PWM2MINvalue = 211;
      break;
    case 18:
      PWM1MAXvalue = 240;
      PWM2MAXvalue = 240;
      PWM2MINvalue = 225;
      PWM2MINvalue = 225;
      break;
    case 19:
      PWM1MAXvalue = 250;
      PWM2MAXvalue = 250;
      PWM2MINvalue = 241;
      PWM2MINvalue = 241;
      break;
    case 20:
      PWM1MAXvalue = 255;
      PWM2MAXvalue = 255;
      PWM2MINvalue = 245;
      PWM2MINvalue = 245;
      break;
  }

  if ( CSensor1.getCurrent() < (value / 10)) {
    PWM1value = PWM1value + 1;
    PWM1value = constrain(PWM1value, PWM1MINvalue, PWM1MAXvalue);
  }
  /*if ( CSensor1.getCurrent() ==  value) {
    PWM1value = PWM1value;
    PWM1value = constrain(PWM1value, PWM1MINvalue, PWM1MAXvalue);
    }*/
  if ( CSensor1.getCurrent() >  (value / 10)) {
    PWM1value = PWM1value - 1;
    PWM1value = constrain(PWM1value, PWM1MINvalue, PWM1MAXvalue);
  }
  analogWrite(CurrentLimter1 , PWM1value);
  //Serial.println(String("PWM") + String(PWM1value) + String(" , ") +  String("CSensor")+String(CSensor1.getCurrent()));
  //------------------------------------------------------------------------
  if ( CSensor2.getCurrent() < (value / 10)) {
    PWM2value = PWM2value + 1;
    PWM2value = constrain(PWM2value, PWM2MINvalue, PWM2MAXvalue);
  }
  /*if ( CSensor2.getCurrent() ==  value) {
    PWM2value = PWM2value;
    PWM2value = constrain(PWM2value, PWM2MINvalue, PWM2MAXvalue);
    }*/
  if ( CSensor2.getCurrent() >  (value / 10)) {
    PWM2value = PWM2value - 1;
    PWM2value = constrain(PWM2value, PWM2MINvalue, PWM2MAXvalue);
  }
  analogWrite(CurrentLimter2  , PWM2value);

}

void setPWM(float value) {

  analogWrite(CurrentLimter1, 60.71 * value);
  analogWrite(CurrentLimter2, 60.71 * value);

}
void Emergency() {
  analogWrite(CurrentLimter1 , 0);
  analogWrite(CurrentLimter2, 0);
}


void CurrentSenserAndLimiterBegin() {
  CSensor1.setSensitivity(1);
  CSensor2.setSensitivity(1);
  pinMode(CurrentLimter1 , OUTPUT);
  pinMode(CurrentLimter2 , OUTPUT);
  PWMFrequncy();
  setALLON();
  delay(4000);
}

#endif
