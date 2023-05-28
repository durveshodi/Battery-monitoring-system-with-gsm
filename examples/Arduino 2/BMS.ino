#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EasyTransfer.h>
#include "Display.h"
#include "Relays.h"
#include "pwm.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
//------------------------------------------------------------------------
#define Buzzer 4
#define RotaryPin1 12
#define RotaryPin2 11
#define RotaryButton 13
Encoder Display(RotaryPin1, RotaryPin2 , RotaryButton, Buzzer);
//------------------------------------------------------------------------
int menu = 1;
int maxmenu = 1;
int minimenu = 1;
//------------------------------------------------------------------------
VariableScreen screens(0, 3, 3000);
//------------------------------------------------------------------------
float CC = 0;
float OV = 0;
float DC = 0;
float UV = 0;
float wh = 0;
float OTP = 0;

int Number0;
int Number1;
int Number2;
int Number3;
int Number4;
int Number5;
int Number6;
int Number7;
int Number8;
int Number9;

int CallTime;

//-----------------------------------------------------------------------
float LastCC;
float LastOV;
float LastDC;
float LastUV;
float Lastwh;
float LastOTP;
int LastCallTime;

int Mode;

int LastNumber0;
int LastNumber1;
int LastNumber2;
int LastNumber3;
int LastNumber4;
int LastNumber5;
int LastNumber6;
int LastNumber7;
int LastNumber8;
int LastNumber9;

float Lastwh1;
float Lastwh2;
//------------------------------------------------------------------------
#define CCAddress 1
#define OVAddress 2
#define DCAddress 3
#define UVAddress 4
#define whAddress 5
#define OTPAddress 6
/*-----------------------*/
#define Digit0Address 11
#define Digit1Address 12
#define Digit2Address 13
#define Digit3Address 14
#define Digit4Address 15
#define Digit5Address 16
#define Digit6Address 17
#define Digit7Address 18
#define Digit8Address 19
#define Digit9Address 20
//#define LastCallTime 21


EasyTransfer ETin, ETout;
float WH, WH1, WH2;
struct RECEIVE_DATA_STRUCTURE {
  /*----- Sensor -----*/
  float Voltage1;
  float Voltage2;
  float Current1;
  float Current2;
  float Temperature1;
  float Temperature2;
  /*---- Limiters -----*/
  float CC;
  float OV;
  float DC;
  float UV;
  float OTP;
  /*-- WH, WH1, WH2 ---*/
  float wh , wh1, wh2;

  bool received;
  bool receivedCapacity;
};


struct SEND_DATA_STRUCTURE {
  /*-------*/
  int MODE;
  /*-------*/
  int number[10];
  int GSMcallTime;
  /*-------*/
  bool Send;
  float CC;
  float OV;
  float DC;
  float UV;
  float wh;
  float OTP;
  /*-------*/
  bool SendCapacity;
  int BatteryCapacity1;
  int BatteryCapacity2;
};

RECEIVE_DATA_STRUCTURE rxDATA;
SEND_DATA_STRUCTURE txDATA;

void ReceiveDATA() {
  if ( ETin.receiveData() == 1) {
    rxDATA.Temperature1;
    rxDATA.Temperature2;
    rxDATA.Voltage1 ;
    rxDATA.Voltage2 ;
    rxDATA.Current1 ;
    rxDATA.Current2;

    rxDATA.CC;
    rxDATA.OV;
    rxDATA.DC;
    rxDATA.UV;
    rxDATA.OTP;

    rxDATA.wh;
    rxDATA.wh1;
    rxDATA.wh2;

    rxDATA.received;
    rxDATA.receivedCapacity;
  }
}

void SendtoWeb(bool value , short int i) {
  switch (i) {
    case 1:
      txDATA.Send = value;
      txDATA.CC = CC;
      txDATA.OV = OV;
      txDATA.DC = DC;
      txDATA.UV = UV;
      txDATA.wh = WH;
      txDATA.OTP = OTP;
      ETout.sendData();
      break;
    case 2:
      txDATA.SendCapacity = value;
      txDATA.BatteryCapacity1;
      txDATA.BatteryCapacity2;
      ETout.sendData();
      break;
  }
}

void Modes(short int value) {
  txDATA.MODE = value;
  if ( txDATA.MODE == 3) {
    txDATA.number[0] = Number0;
    txDATA.number[1] = Number1;
    txDATA.number[2] = Number2;
    txDATA.number[3] = Number3;
    txDATA.number[4] = Number4;
    txDATA.number[5] = Number5;
    txDATA.number[6] = Number6;
    txDATA.number[7] = Number7;
    txDATA.number[8] = Number8;
    txDATA.number[9] = Number9;
    txDATA.number[10];
    txDATA.GSMcallTime = CallTime;
    ETout.sendData();
  }
}



void StoreFloatDATA(const int Adddress , float value) {
  String Value = "";
  Value.concat(value);
  String NewValue = String(".") +  Value + String(".");
  int delimiter, delimiter_1, delimiter_2;
  delimiter = NewValue.indexOf(".");
  delimiter_1 = NewValue.indexOf(".", delimiter + 1);
  delimiter_2 = NewValue.indexOf(".", delimiter_1 + 1);
  String value1 = NewValue.substring(delimiter + 1, delimiter_1);
  String value2 = NewValue.substring(delimiter_1 + 1, delimiter_2);
  int i1 = value1.toInt();
  int i2 = value2.toInt();
  EEPROM.write(Adddress,  i1);
  EEPROM.write(Adddress + 256, i2);
}

float ReadFloatDATA(const int Adddress) {
  int i1 = EEPROM.read(Adddress);
  int i2 = EEPROM.read(Adddress + 256);
  String Float = {String(i1) + String(".") + String(i2)};
  float NewValue = Float.toFloat();
  return NewValue;
}


void setup() {
  lcd.begin();
  lcd.backlight();
  EEPROM.begin();
  updateMenu();

  Serial.begin(9600);

  ETin.begin(details(rxDATA), &Serial);
  ETout.begin(details(txDATA), &Serial);

  pinMode(smps , OUTPUT);

  pinMode(CurrentLimter1 , OUTPUT);
  pinMode(CurrentLimter2 , OUTPUT);
  PWMFrequncy();
  setALLON();

  delay(4000);

  pinMode(relay1 , OUTPUT);
  pinMode(relay2 , OUTPUT);
  pinMode(relay3 , OUTPUT);
  pinMode(relay4 , OUTPUT);
  pinMode(relay5 , OUTPUT);
  RelayALL(OFF);


  Mode = 2  ;
  SMPS(OFF);
  delay(2000);
  RelayALL(OFF);
  delay(2000);

  CSensor1.setSensitivity(1);
  CSensor2.setSensitivity(1);
  //---------------------------------------------
  LastCC = ReadFloatDATA(1);
  LastOV = ReadFloatDATA(2);
  LastDC = ReadFloatDATA(3);
  LastUV = ReadFloatDATA(4);
  Lastwh = ReadFloatDATA(5);
  LastOTP = ReadFloatDATA(6);

  LastNumber0 = EEPROM.read(11);
  LastNumber1 = EEPROM.read(12);
  LastNumber2 = EEPROM.read(13);
  LastNumber3 = EEPROM.read(14);
  LastNumber4 = EEPROM.read(15);
  LastNumber5 = EEPROM.read(16);
  LastNumber6 = EEPROM.read(17);
  LastNumber7 = EEPROM.read(18);
  LastNumber8 = EEPROM.read(19);
  LastNumber9 = EEPROM.read(20);

  LastCallTime = EEPROM.read(21);

  CC = LastCC;
  OV = LastOV;
  DC = LastDC;
  UV = LastUV;
  wh = Lastwh;
  OTP = LastOTP;

  Number0 = LastNumber0;
  Number1 = LastNumber1;
  Number2 = LastNumber2;
  Number3 = LastNumber3;
  Number4 = LastNumber4;
  Number5 = LastNumber5;
  Number6 = LastNumber6;
  Number7 = LastNumber7;
  Number8 = LastNumber8;
  Number9 = LastNumber9;

  CallTime = LastCallTime;
}

void NewdataRecevied() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DATA Recevided" );
  delay(2000);
}

int emg = 0;
//-----------------------------------------------------------------------------------------------------------

void CheckAnyChange() {
  //--------------------------------------------------
  if (rxDATA.CC != CC && rxDATA.CC > 0) {
    NewdataRecevied();
    CC = rxDATA.CC;
  }
  if (rxDATA.OV != OV && rxDATA.OV > 0) {
    StoreFloatDATA( 2 , rxDATA.OV);
    NewdataRecevied();
    OV = rxDATA.OV;
  }
  if (rxDATA.DC != DC && rxDATA.DC > 0) {
    StoreFloatDATA( 3 , rxDATA.DC);
    NewdataRecevied();
    DC = rxDATA.DC;
  }
  if (rxDATA.UV != UV && rxDATA.UV > 0) {
    StoreFloatDATA( 4 , rxDATA.UV);
    NewdataRecevied();
    UV = rxDATA.UV;
  }
  if (rxDATA.wh != wh && rxDATA.wh > 0) {
    StoreFloatDATA( 5 , rxDATA.wh);
    NewdataRecevied();
    wh = rxDATA.wh;
  }
  if (rxDATA.OTP != OTP && rxDATA.OTP > 0) {
    StoreFloatDATA( 6 , rxDATA.OTP);
    NewdataRecevied();
    OTP = rxDATA.OTP;
  }
  //----------------------------------------------
  if (CC != LastCC) {
    StoreFloatDATA( 1 , CC);
    LastCC = CC;
  }
  if ( OV != LastOV) {
    StoreFloatDATA( 2 , OV);
    LastOV = OV;;
  }
  if ( DC != LastDC) {
    StoreFloatDATA( 3 , DC);
    LastDC = DC;
  }
  if ( UV != LastUV) {
    StoreFloatDATA( 4 , UV);
    LastUV = UV;
  }
  if ( wh != Lastwh) {
    StoreFloatDATA( 5 , wh);
    Lastwh = wh;
  }
  if ( OTP != LastOTP) {
    StoreFloatDATA( 6 , OTP);
    LastOTP = OTP;
  }
  //------------------------------------------------------
  if (Number0 != LastNumber0) {
    EEPROM.write(11, Number0);
    LastNumber0 = Number0;
  }
  if (Number1 != LastNumber1) {
    EEPROM.write(12 , Number1);
    LastNumber1 = Number1;
  }
  if (Number2 != Number2) {
    EEPROM.write(13 , Number2);
    LastNumber2 = Number2;
  }
  if (Number3 != LastNumber3) {
    EEPROM.write(14 , Number3);
    LastNumber3 = Number3;
  }
  if (Number4 != LastNumber4) {
    EEPROM.write(15 , Number4);
    LastNumber4 = Number4;
  }
  if (Number5 != LastNumber5) {
    EEPROM.write(16 , Number5);
    LastNumber5 = Number5;
  }
  if (Number6 != LastNumber6) {
    EEPROM.write(17 , Number6);
    LastNumber6 = Number6;
  }
  if (Number7 != LastNumber7) {
    EEPROM.write(18 , Number7);
    LastNumber7 != Number7;
  }
  if (Number8 != LastNumber8) {
    EEPROM.write(19 , Number8);
    LastNumber8 = Number8;
  }
  if (Number9 != LastNumber9) {
    EEPROM.write(20 , Number9);
    LastNumber9 = Number9;
  }
  // -----------------------------------------------
  if (CallTime != LastCallTime) {
    EEPROM.write(21 , CallTime);
    LastCallTime = CallTime;
  }
}
//------------------------------------------------------------------

float LastVoltage1 = 0;
float LastVoltage2 = 0;
short intNumber = 2;
long int CurrentSendedTime = 0 ;

bool SendDATA = false;
bool Conditions;

long int StopTime = 0;
int notetime = 0;
long int CurrnetCheckMillis = 0;
float voltage1 = 0;
bool CurrentStatus1 = true;
bool CurrentStatus2 = true;

short int bufferrelay = 0;
long int LastCurrnetCheckMillis = 0;

bool  buttonpress = false;
//-----------------------------------------------------------------------------------------------------------
void loop() {
  //-------------------------------------------------------------------------
  if (intNumber == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sending DATA to");
    lcd.setCursor(0, 1);
    lcd.print("Web Server");
    intNumber = 2;
  }
  //=========================================
  CurrentSendedTime = millis();
  while (SendDATA == true) { // millis()/ 1000 is ffor convert millis into seconds
    ReceiveDATA();
    Display.updateBuzzer();
    SendtoWeb(true , 1);
    if (rxDATA.received == true ) {
      notetime = millis() - CurrentSendedTime;
      SendtoWeb(false , 1);
      if (rxDATA.received == true  && notetime > 10000) {
        //Serial.println(String("RECEIVE Time : ") + String(millis() - CurrentSendedTime));
        Conditions = true;
        SendDATA = false;
        break;
      }
    }
  }
  //-------------------------- BUFFER -------------------------------
  while (Conditions == true ) {
    if (StopTime == 0) {
      StopTime = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Successfully" );
      lcd.setCursor(0, 1);
      lcd.print("Sended to Web");//)String( + String(notetime / 1000) + String("S") );
    }
    if (millis() - StopTime < 20000) {
      ReceiveDATA();
      SendtoWeb(false , 1);
    } else {
      StopTime = 0;
      Conditions = false;
      break;
    }
  }
  //----------------------------------------------------------
  if (SendDATA == false && Conditions == false ) {
    if (emg != 1) {
      Display.updateDisplay(updateMenu , menu);
      Display.updateBuzzer();
      Display.executeAction( updateMenu, Action , menu);
      screens.refreshs(menu, screen);
    }
    // ---------------------------------------------
    if (menu == 1) {
      ReceiveDATA();
      SendtoWeb(true , 2);
      CheckAnyChange();
      //-------------------------------  Battery Levels and Voltage -------------------------------
      if ((voltage1 > 12.5 || rxDATA.Voltage2 > 12.5) && ( Mode == 2)) {
        txDATA.BatteryCapacity1 = 100;
        txDATA.BatteryCapacity2 = 100;
      }
      else if (Mode == 1) {
        txDATA.BatteryCapacity1 = 0;
        txDATA.BatteryCapacity2 = 0;
      }
      else {
        txDATA.BatteryCapacity1 = (((voltage1 -  UV) * 100) / (12.5 - UV));
        txDATA.BatteryCapacity2 = (((rxDATA.Voltage2 -  UV) * 100) / (12.5 - UV));
      }
      //--------------------------------------------------
      if (rxDATA.Voltage1 > 0 && rxDATA.Voltage2 >  0 ) {
        if ( rxDATA.Voltage1   > 15) {
          voltage1 = rxDATA.Voltage1  - rxDATA.Voltage2;
        }
        else {
          voltage1 = rxDATA.Voltage1;
        }
      }
      if (rxDATA.Voltage1 == 0 && rxDATA.Voltage2 ==  0 ) {
        SMPS(ON);
      }
      //------------------------------- Charging Mode -------------------------------

      if ( Mode == 1) { // Charging Mode
        if (bufferrelay == 0) {
          SMPS(ON);
          delay(2000);
          Battery1(ON);
          Battery2(ON);
          DischargeRelay(OFF);
          bufferrelay = 1;
        }

        Modes(1);
        setCurrent(CC);

        if (  voltage1 > OV  ) {
          LastVoltage1 = voltage1 ;
          Battery1(OFF);
        }
        if ( rxDATA.Voltage2 > OV) {
          LastVoltage2 = rxDATA.Voltage2 ;
          Battery2(OFF);
        }

        if (LastVoltage1 >= OV && LastVoltage2 >= OV ) { // Charging Mode
          Modes(2);
          Mode =  2;
          RelayALL(OFF);
          delay(2000);
          SMPS(OFF);
          bufferrelay = 0;
        }

        //------------------------------- Current Protection -------------------------------
        CurrnetCheckMillis = millis();
        if (CurrnetCheckMillis - LastCurrnetCheckMillis >= 60000 ) {
          LastCurrnetCheckMillis = CurrnetCheckMillis;
          if (0 < rxDATA.Current1  || 0 < rxDATA.Current2 ) {

            if ( 0.05 > rxDATA.Current1  ) {
              CurrentStatus1 = false;
              Battery1(OFF);
            } else {
              CurrentStatus1 = true;
            }

            if ( 0.05 > rxDATA.Current2) {
              CurrentStatus2 = false;
              Battery2(OFF);
            } else {
              CurrentStatus2 = true;
            }

            if (CurrentStatus1 == false && CurrentStatus2 == false) {
              Modes(2);
              Mode =  2;
              RelayALL(OFF);
              delay(2000);
              SMPS(OFF);
              bufferrelay = 0;
            }
          }
        }
      }
      //------------------------------- Discharge Mode -------------------------------


      if (UV > voltage1  || UV > rxDATA.Voltage2 ) {
        Modes(1);
        Mode =  1;
        SMPS(ON);
        delay(2000);
        RelayALL(ON);
      }
      if ( Mode ==  2) { // Discharge Mode

        Modes(2);
        setCurrent(DC);
        DischargeRelay(ON);
      }
      //--------------------- Emergency Mode ---------------------------
      if ( rxDATA.Temperature1 > OTP || rxDATA.Temperature2 > OTP) {
        long int LoopTime = 0;
        int MinimunTime  = millis();
        while (true) {
          if (emg == 0) {
            //--------------------
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("_Emergency Mode_" );
            Emergency();
            SMPS(ON);
            delay(2000);
            RelayALL(OFF);
            delay(2000);
            Battery1(OFF);
            Battery2(OFF);
            DischargeRelay(OFF);
            //--------------------
            emg = 1;
          }

          ReceiveDATA();
          if (millis() - MinimunTime >= 100) {
            MinimunTime = millis();
            Modes(3);
          }
          //---------------------------------------
          if (digitalRead(RotaryButton) == HIGH) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Please Wailt" );
            //--------------------
            RelayALL(OFF);
            delay(2000);
            SMPS(OFF);
            DischargeRelay(ON);
            //--------------------
            LoopTime = millis();
            buttonpress = true;
          }
          if (buttonpress == true) {
            while (true) {
              Modes(2);
              Mode = 2;
              txDATA.OTP = 50;
              ETout.sendData();
              emg = 0;
              if (millis() - LoopTime >= 5000) {
                StoreFloatDATA( 6 , 50);
                delay(1000-);
                break;
              }
            }
            buttonpress = false;
            break;
          }
        }
      }
    }
  }
}

//----------------------------------------------------------------------------------------
void screen() {
  if (screens.Change() == 0) {
    if (Mode == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Charging Mode B1");
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BATTERY");
      lcd.setCursor(12, 0);
      lcd.print(String(txDATA.BatteryCapacity1) + String("%") );

    }
    lcd.setCursor(0, 1);
    lcd.print("TEMPERATURE");
    lcd.setCursor(12, 1);
    lcd.print( rxDATA.Temperature1);
  }
  if (screens.Change() == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CURRENT");
    lcd.setCursor(12, 0);
    lcd.print(rxDATA.Current1);
    lcd.setCursor(0, 1);
    lcd.print("VOLTAGE");
    lcd.setCursor(12, 1);
    lcd.print(voltage1);
  }
  if (screens.Change()  == 2) {

    if (Mode == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Charging Mode B2");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BATTERY 2");
      lcd.setCursor(12, 0);
      lcd.print(String(txDATA.BatteryCapacity2) + String("%") );
    }
    lcd.setCursor(0, 1);
    lcd.print("TEMPERATURE");
    lcd.setCursor(12, 1);
    lcd.print( rxDATA.Temperature2);
  }
  if (screens.Change() == 3) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CURRENT");
    lcd.setCursor(12, 0);
    lcd.print(rxDATA.Current2);
    lcd.setCursor(0, 1);
    lcd.print("VOLTAGE");
    lcd.setCursor(12, 1);
    lcd.print(rxDATA.Voltage2);
  }
}
//----------------------------------------------------------------------------------------


void updateMenu() {
  //Serial.println(menu);
  menu = Display.rotateMenu(menu , maxmenu , minimenu);
  switch (menu) {
    //------------------------ Layer 1 --------------------------------
    case 1:
      break;
    //------------------------ Layer 2 --------------------------------
    case 2:
      lcd.clear();
      lcd.print(">Setting");
      lcd.setCursor(0, 1);
      lcd.print(" Back");
      break;
    case 3:
      lcd.clear();
      lcd.print(" Setting");
      lcd.setCursor(0, 1);
      lcd.print(">Back");
      break;
    //------------------------ Layer 3 --------------------------------
    case 21:
      lcd.clear();
      lcd.print(">CHARGING");
      lcd.setCursor(0, 1);
      lcd.print(" Setting");
      break;
    case 22:
      lcd.clear();
      lcd.print(">DISCHARGING");
      lcd.setCursor(0, 1);
      lcd.print(" Setting");
      break;

    case 23:
      lcd.clear();
      lcd.print(">TEMPERATURE");
      lcd.setCursor(0, 1);
      lcd.print(" Setting");
      break;

    case 24:
      lcd.clear();
      lcd.print(">GSM Setting");
      lcd.setCursor(0, 1);
      lcd.print(" Other Setting");
      break;

    case 25:
      lcd.clear();
      lcd.print(" GSM SETTING");
      lcd.setCursor(0, 1);
      lcd.print(">Other Setting");
      break;

    case 26:
      lcd.clear();
      lcd.print(">Credit");
      lcd.setCursor(0, 1);
      lcd.print(" Back");
      break;

    case 27:
      lcd.clear();
      lcd.print(" Credit");
      lcd.setCursor(0, 1);
      lcd.print(">Back");
      break;
    //------------------------ Layer 4x01 --------------------------------
    case 211:
      CC = Display.ConfigsValue(CC , 2 , 0 , 0.1 );
      lcd.clear();
      lcd.print(String("CC Setting >") + CC);
      lcd.setCursor(0, 1);
      lcd.print(String("OV Setting  ") + OV);
      //lcd.print(String("        ") + String("  =>") + CC);
      break;

    case 212:
      OV = Display.ConfigsValue(OV , 15 , 0 , 0.1 );
      lcd.clear();
      // lcd.print("CC Setting");
      lcd.print(String("CC Setting  ") + CC);
      lcd.setCursor(0, 1);
      lcd.print(String("OV Setting >") + OV);
      break;
    //------------------------
    case 213:
      lcd.clear();
      lcd.print(">BACK");
      break;
    //------------------------ Layer 4x02 --------------------------------
    case 221:
      DC = Display.ConfigsValue(DC , 2 , 0 , 0.1);
      lcd.clear();
      lcd.print(String("DC Setting >") + DC);
      lcd.setCursor(0, 1);
      lcd.print(String("UV Setting  ") + UV);
      break;

    case 222:
      UV = Display.ConfigsValue(UV , 8 , 0 , 0.1);
      lcd.clear();
      lcd.print(String("DC Setting  ") + DC);
      lcd.setCursor(0, 1);
      lcd.print(String("UV Setting >") + UV);
      break;
    //------------------------
    case 223:
      lcd.clear();
      lcd.print(">BACK");
      break;
    //------------------------ Layer 4x03 --------------------------------
    case 231:
      OTP = Display.ConfigsValue(OTP , 100 , 0 , 1);
      lcd.clear();
      lcd.print(String("OTP Setting>") + OTP);
      lcd.setCursor(0, 1);
      lcd.print("            BACK");
      break;
    //------------------------
    case 232:
      lcd.clear();
      lcd.print(String("OTP Setting ") + OTP);
      lcd.setCursor(0, 1);
      lcd.print("           >BACK");
      break;
    //------------------------ Layer 4x04x01 --------------------------------
    case 241:
      Number0 = Display.ConfigsValue(Number0 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(">") + String(Number0) + String(Number1) + String(Number2) + String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));
      break;

    case 242:
      Number1 = Display.ConfigsValue(Number1 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(">")  + String(Number1) + String(Number2) + String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));

      break;
    case 243:
      Number2 = Display.ConfigsValue(Number2 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(">")  +  String(Number2) + String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));

      break;
    case 244:
      Number3 = Display.ConfigsValue(Number3 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2) + String(">")  +  String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));
      break;

    case 245:
      Number4 = Display.ConfigsValue(Number4 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2) +   String(Number3) + String(">") + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));

      break;
    case 246:
      Number5 = Display.ConfigsValue(Number5 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2) + String(Number3) + String(Number4) +  String(">") + String(Number5) + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));
      break;

    case 247:
      Number6 = Display.ConfigsValue(Number6 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2) +  String(Number3) + String(Number4) + String(Number5) + String(">")  + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));
      break;

    case 248:
      Number7 = Display.ConfigsValue(Number7 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2)  +  String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(">") + String(Number7) + String(Number8) + String(Number9) + String(" BACK"));
      break;
    case 249:
      Number8 = Display.ConfigsValue(Number8 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2) +  String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(">") + String(Number8) +  String(Number9) + String(" BACK"));
      break;
    case 250:
      Number9 = Display.ConfigsValue(Number9 , 9 , 0 , 1 );
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2) + String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(Number8) +  String(">") + String(Number9) + String(" BACK"));
      break;
    //------------------------
    case 251:
      lcd.clear();
      lcd.print(" NUMBER");
      lcd.setCursor(0, 1);
      lcd.print(String(Number0) + String(Number1) + String(Number2) + String(Number3) + String(Number4) + String(Number5) + String(Number6) + String(Number7) + String(Number8) + String(Number9) + String(" >BACK"));
      break;
    //-----------------------------------------------------------------------------------------------    case 221:
    case 252:
      wh = Display.ConfigsValue(wh , 10000 , 100 , 1);
      lcd.clear();
      lcd.print(String("Wh      >") + wh);
      lcd.setCursor(0, 1);
      lcd.print(String("CallTime ") + CallTime);
      break;

    case 253:
      CallTime = Display.ConfigsValue(CallTime , 120 , 1 , 1);
      lcd.clear();
      lcd.print(String("Wh     ") + wh);
      lcd.setCursor(0, 1);
      lcd.print(String("CallTim>") + CallTime);
      break;
    //------------------------
    case 254:
      lcd.clear();
      lcd.print(">BACK");
      break;

    //------------------------
    case 261:
      lcd.clear();
      lcd.print("Created By     ");
      lcd.setCursor(0, 1);
      lcd.print("        Durvesh");
      break;
  }
}

void Action() {
  switch (menu) {
    //--------------- 1 Layer  ---------------
    case 1:
      if ( OTP ) {
        maxmenu = 3;
        minimenu = 2;
        menu = 2;
      }
      break;
    //--------------- 2 Layer  ---------------
    case 2:
      maxmenu = 27;
      minimenu = 21;
      menu = 21;
      break;
    case 3:
      maxmenu = 1;
      minimenu = 1;
      menu = 1;
      SendDATA = true;
      intNumber = 1;
      break;

    //------------------------ Layer 3 --------------------------------

    //------------------------ Layer 3 --------------------------------
    case 21:
      minimenu = 211;
      maxmenu = 213;
      menu = 211;
      break;
    case 22:
      minimenu = 221;
      maxmenu = 223;
      menu = 221;
      break;

    case 23:
      minimenu = 231;
      maxmenu = 232;
      menu = 231;
      break;
    case 24:
      minimenu = 241;
      maxmenu = 251;
      menu = 231;
      break;
    case 25:
      minimenu = 252;
      maxmenu = 254;
      menu = 252;
      break;
    case 26:
      minimenu = 261;
      maxmenu = 261;
      menu = 261;
      break;
    //------------------------
    case 27:
      minimenu = 2;
      maxmenu = 3;
      menu = 2;
      break;
    //------------------------ Layer 4x01 --------------------------------
    case 211:
      Display.configMode();
      break;
    case 212:
      Display.configMode();
      break;
    //------------------------
    case 213:
      minimenu = 21;
      maxmenu = 27;
      menu = 21;
      break;
    //------------------------ Layer 4x02 --------------------------------
    case 221:
      Display.configMode();
      break;
    case 222:
      Display.configMode();
      break;
    //------------------------
    case 223:
      minimenu = 21;
      maxmenu = 27;
      menu = 22;
      break;
    //------------------------ Layer 4x03 --------------------------------
    case 231:
      Display.configMode();
      break;
    //------------------------
    case 232:
      minimenu = 21;
      maxmenu = 27;
      menu = 23;
      break;
    //------------------------ Layer 4x04 --------------------------------
    case 241:
      Display.configMode();
      break;
    case 242:
      Display.configMode();
      break;
    case 243:
      Display.configMode();
      break;
    case 244:
      Display.configMode();
      break;
    case 245:
      Display.configMode();
      break;
    case 246:
      Display.configMode();
      break;
    case 247:
      Display.configMode();
      break;
    case 248:
      Display.configMode();
      break;
    case 249:
      Display.configMode();
      break;
    case 250:
      Display.configMode();
      break;
    //------------------------
    case 251:
      minimenu = 21;
      maxmenu = 27;
      menu = 24;
      break;

    //------------------------

    case 252:
      Display.configMode();
      break;
    case 253:
      Display.configMode();
      break;
    //------------------------
    case 254:
      minimenu = 21;
      maxmenu = 27;
      menu = 25;
      break;

    //------------------------
    case 261:
      minimenu = 21;
      maxmenu = 27;
      menu = 26;
      break;
      //------------------------
  }
}
