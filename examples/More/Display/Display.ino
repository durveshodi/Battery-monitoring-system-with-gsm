#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Display.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//------------------------------------------------------------------------
#define CLK 11
#define DT 12
int Button = 13;
Encoder Display(DT, CLK , 13, 4);
//------------------------------------------------------------------------
int menu = 1;
int maxmenu = 1;
int minimenu = 1;
//------------------------------------------------------------------------
VariableScreen screens(0, 3, 3000);
//------------------------------------------------------------------------
float Temp1, Voltage1, Current1;
float Temp2, Voltage2, Current2;
//------------------------------------------------------------------------

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

//-----------------------------------------------------------------------
float LastCC;
float LastOV;
float LastDC;
float LastUV;
float Lastwh;
float LastOTP;
int LastCallTime;


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

  Serial.println(i1);
  Serial.println(i2);
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
  updateMenu();
  EEPROM.begin();
  Serial.begin(9600);

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
//-----------------------------------------------------------------------------------------------------------

void CheckAnyChange() {
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


void loop() {
      Display.updateDisplay(updateMenu , menu);
      Display.updateBuzzer();
      Display.executeAction( updateMenu, Action , menu);
      screens.refreshs(menu, screen);
      CheckAnyChange();}
//----------------------------------------------------------------------------------------
void screen() {
  if (
    screens.Change() == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BATTERY");
    lcd.setCursor(12, 0);
    lcd.print("100%");
    lcd.setCursor(0, 1);
    lcd.print(String("TEMPERATURE"));
    lcd.setCursor(12, 1);
    lcd.print(Temp1);
  }
  if (screens.Change() == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(String("CURRENT"));
    lcd.setCursor(12, 0);
    lcd.print(Current1);
    lcd.setCursor(0, 1);
    lcd.print(String("VOLTAGE"));
    lcd.setCursor(12, 1);
    lcd.print(Voltage1);
  }
  if (screens.Change()  == 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BATTERY 2");
    lcd.setCursor(12, 0);
    lcd.print("100%");
    lcd.setCursor(0, 1);
    lcd.print(String("TEMPERATURE"));
    lcd.setCursor(12, 1);
    lcd.print(Temp2);
  }
  if (screens.Change() == 3) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(String("CURRENT"));
    lcd.setCursor(12, 0);
    lcd.print(Current2);
    lcd.setCursor(0, 1);
    lcd.print(String("VOLTAGE"));
    lcd.setCursor(12, 1);
    lcd.print(Voltage2);
  }
}
//----------------------------------------------------------------------------------------

void updateMenu() {
  Serial.println(menu);
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
      // lcd.print("CC Setting");
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
      /*lcd.clear();
        lcd.print("OCV Setting");
        lcd.setCursor(0, 1);
        lcd.print(String("        ") + String("  =>") + OCV);*/
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
      maxmenu = 3;
      minimenu = 2;
      menu = 2;
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
