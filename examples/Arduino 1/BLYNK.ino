#define BLYNK_PRINT Serial
#define BLYNK_MSG_LIMIT  200
#define BLYNK_TEMPLATE_ID "TMPLeT3KK3iT"
#define BLYNK_DEVICE_NAME "Wireless Battery Monitoring System"
#define BLYNK_AUTH_TOKEN "Ll43AvEcq-DfkBD6s8_Wx0hA4Gx2F9u7"
//#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800
#define BLYNK_HEARTBEAT 30
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <SoftwareSerial.h>
#include <SoftEasyTransfer.h>
SoftwareSerial GSMSerial(6, 5); // RX, TX
TinyGsm modem(GSMSerial);
SoftwareSerial A2ASerial(8, 7);
SoftEasyTransfer ETin, ETout;

char auth[] = BLYNK_AUTH_TOKEN;
char apn[]  = "VI";
char user[] = "";
char pass[] = "";

WidgetLED led1(V14);
WidgetLED led2(V15);
/*---------------------------------*/
//#define BLYNK_GREEN     "#23C48E"
//#define BLYNK_YELLOW    "#ED9D00"
//#define BLYNK_RED       "#D3435C"
/*---------------------------------*/
#include <themistor_sensor.h>
Temparature_Senser TSensor1 (A5, 9740);
Temparature_Senser TSensor2 (A4, 9650);
/*---------------------------------*/
#include <Voltage_sensor.h>
Voltage_Sensor VSensor1 (A2 , 9820 , 975); // pin , R1 ,R2
Voltage_Sensor VSensor2 (A3 , 9750 , 950); // pin , R1 ,R2
/*---------------------------------*/
#include <Current_sensor.h>
Current_sensor CSensor1 (A0); // pin
Current_sensor CSensor2 (A1); // pin
//----------------- variabes ------------------------------

float WH = 0; // totle watt per hour
float WH1; // Battery 1 Watt per Hour
float WH2; // Battery 2 Watt per Hour
float changingWH1;
float changingWH2;
//--------------------
float Temperature11;
float Temperature22;
float Voltage11;
float Voltage22;
float Current11;
float Current22;
//--------------------
float oldTemperature1;
float oldTemperature2;
float oldVoltage1;
float oldVoltage2;
float oldCurrent1;
float oldCurrent2;
float oldCC;
float oldOV;
float oldDC;
float oldUV;
float oldwh;
float oldOTP;

/*---------------------------------*/
long int TimerMillis = 0;
long int callingTimerMillis = 0;

//-----------------------------------------------------

struct RECEIVE_DATA_STRUCTURE {
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
struct SEND_DATA_STRUCTURE {
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
  /*--------------------*/

};

RECEIVE_DATA_STRUCTURE rxDATA;
SEND_DATA_STRUCTURE txDATA;
//---------------------------------------------------------
// SendandReceiveDATAfromArduino
void SendandReceiveDATAfromArduino( ) {
  A2ASerial.listen();

  txDATA.Temperature1 = Temperature11;
  txDATA.Temperature2 = Temperature22;
  txDATA.Voltage1 = Voltage11;
  txDATA.Voltage2 = Voltage22;
  txDATA.Current1 = Current11;
  txDATA.Current2 = Current22;
  /*--------------------------------------------------------*/
  txDATA.CC;
  txDATA.OV;
  txDATA.DC;
  txDATA.UV;
  txDATA.OTP;
  txDATA.received;
  txDATA.receivedCapacity;
  txDATA.wh = WH;
  txDATA.wh1 = WH1;
  txDATA.wh2 = WH2;
  /*--------------------------------*/
  ETout.sendData();

  //--------------------------------------------------------
  ETin.receiveData();
  rxDATA.MODE;
  rxDATA.number[0];
  rxDATA.number[1];
  rxDATA.number[2];
  rxDATA.number[3];
  rxDATA.number[4];
  rxDATA.number[5];
  rxDATA.number[6];
  rxDATA.number[7];
  rxDATA.number[8];
  rxDATA.number[9];

  rxDATA.Send;
  rxDATA.CC;
  rxDATA.OV;
  rxDATA.DC;
  rxDATA.UV;
  rxDATA.wh;
  rxDATA.OTP;

  rxDATA.BatteryCapacity1;
  rxDATA.BatteryCapacity2;
  /*--------------------------------------------------------*/

}
long int SendTAG = 0;
long int LastSendedTAG = 0;
bool lastSend = false;

int CurrentCapacity11 = 0;
int CurrentCapacity22 = 0;
/*--------------------------------------------------------------*/
void blynkdata() {
  /*----------------------------------------------------------*/
  Blynk.virtualWrite(V1, Temperature11);
  Blynk.virtualWrite(V2, Temperature22);
  if (rxDATA.MODE == 1 || rxDATA.MODE == 3) {
    Blynk.virtualWrite(V3, Voltage11);
    Blynk.virtualWrite(V4, Voltage22);
  }
  else {
    Blynk.virtualWrite(V3, Voltage11 - Voltage22); //
    Blynk.virtualWrite(V4, Voltage22);
  }
  Blynk.virtualWrite(V5, Current11);
  Blynk.virtualWrite(V6, Current22);
  if ( rxDATA.BatteryCapacity1 != CurrentCapacity11 && rxDATA.BatteryCapacity1 > 0) {
    Blynk.virtualWrite(V16, rxDATA.BatteryCapacity1);
    CurrentCapacity11 != rxDATA.BatteryCapacity1;
  }
  if ( rxDATA.BatteryCapacity2 != CurrentCapacity22 && rxDATA.BatteryCapacity2 > 0) {
    Blynk.virtualWrite(V17, rxDATA.BatteryCapacity2);
    CurrentCapacity22 != rxDATA.BatteryCapacity2;
  }
  //  Blynk.virtualWrite(V17, rxDATA.BatteryCapacity2);
  //
  /* while (true) {
     SendandReceiveDATAfromArduino( );
     txDATA.receivedCapacity = true;
     if (!Blynk.connected()) {
       Blynk.run();
     }
     if ( rxDATA.SendCapacity == false) {
       break;
     }
    }
    }*/
  /*-----------------------------------------------------------*/
  //Serial.println(rxDATA.Send);  
//  Serial.println(txDATA.received);
  if ( rxDATA.Send == true) {
    //A2ASerial.listen();
    //SendandReceiveDATAfromArduino( ) ;
      if (lastSend == false) {
        Blynk.virtualWrite(V7, rxDATA.CC);
        Blynk.virtualWrite(V9, rxDATA.OV);
        Blynk.virtualWrite(V10, rxDATA.DC);
        Blynk.virtualWrite(V11, rxDATA.UV);
        Blynk.virtualWrite(V20, rxDATA.wh);
        Blynk.virtualWrite(V25, rxDATA.OTP);
        Serial.println("DATA RECEVIED");
        lastSend =  true;
      } else {
        txDATA.received = true;
      }
    } else {
      lastSend =  false;
      txDATA.received = false;
    }
  }
  //---------------------------------------------------------
  BLYNK_WRITE(V7) { // 1
    txDATA.CC = param.asFloat();
  }
  BLYNK_WRITE(V9) { // 2
    txDATA.OV = param.asFloat();
  }
  BLYNK_WRITE(V10) { // 3
    txDATA.DC = param.asFloat();
  }
  BLYNK_WRITE(V11) { // 4
    txDATA.UV = param.asFloat();
  }
  BLYNK_WRITE(V20) { // 5
    txDATA.wh = param.asFloat();
  }
  BLYNK_WRITE(V25) { // 6
    txDATA.OTP = param.asFloat();
  }
  //---------------------------------------------------------
  int lastCapacity1 = 0;
  int lastCapacity2 = 0;

  int CurrentCapacity1 = 0;
  int CurrentCapacity2 = 0;

  void setup() {
    TSensor1.setSensitivity(0.77160493827);//0.61406640486
    TSensor2.setSensitivity(0.65559440559);//0.71001031992
    VSensor1.setSensitivity(0.87126436782);//0.8253
    VSensor2.setSensitivity(0.84787472036);//0.8150
    CSensor1.setSensitivity(1);
    CSensor2.setSensitivity(1);

   // Serial.begin(9600);
    delay(10);
    GSMSerial.begin(9600); // Set GSM module baud rate
    delay(6000);
    //Serial.println("Initializing modem...");
    Blynk.begin(auth, modem, apn, user, pass);

    led1.on();
    led2.on();

    //led1.setColor(BLYNK_GREEN);
    //led2.setColor(BLYNK_GREEN);

    A2ASerial.begin(9600);
    ETin.begin(details(rxDATA), &A2ASerial);
    ETout.begin(details(txDATA), &A2ASerial);

  }

  long int Currentmillies = 0;
  long int LastMillies = 0;
  short int timers = 0;

  //long int end_time = millis() + milli;


  void blinkLedWidget() {
    if (led1.getValue()) {
      led1.off();
      led2.off();
    }
    else {
      led1.on();
      led2.on();
    }
  }
  long unsigned CounterTimer = 0;
  short int Mode = 0;
  float ModeTimer = 0;

  void loop() {
    while (true) {
      Currentmillies = millis();
      if (Currentmillies - LastMillies >= 0) {
        LastMillies = Currentmillies;
        timers = timers + 1;

        oldTemperature1 = (oldTemperature1 + TSensor1.getTempinC());
        oldTemperature2 = oldTemperature2 + TSensor2.getTempinC();
        oldVoltage1 = oldVoltage1 + VSensor1.getVoltage();
        oldVoltage2 = oldVoltage2  + VSensor2.getVoltage();
        oldCurrent1 = oldCurrent1 + CSensor1.getCurrent();
        oldCurrent2 = oldCurrent2 + CSensor2.getCurrent();
        SendandReceiveDATAfromArduino( );

        if (timers == 10) {
          GSMSerial.listen();
          //blynkdata();
          Temperature11 = (oldTemperature1 + Temperature11) / ( timers + 1) ;
          Temperature22 = (oldTemperature2 + Temperature22) / ( timers + 1) ;
          Voltage11 = (oldVoltage1 + Voltage11) / ( timers + 1) ;
          Voltage22 = (oldVoltage2 + Voltage22) / ( timers + 1) ;
          Current11 = (oldCurrent1 + Current11) / ( timers + 1) ;
          Current22 = (oldCurrent2 + Current22) / ( timers + 1) ;
          //txDATA.receivedCapacity = false;
          blynkdata();

          oldTemperature1 = 0;
          oldTemperature2 = 0;
          oldVoltage1 = 0;
          oldVoltage2 = 0;
          oldCurrent1 = 0;
          oldCurrent2 = 0;
          timers = 0;
          break;
          GSMSerial.listen();
        }
      }
    }

    /*  Serial.println(String(rxDATA.MODE) + String(",") +
                     String(rxDATA.Send) + String(",") +
                     String(rxDATA.CC) + String(",") +
                     String(rxDATA.OV) + String(",") +
                     String(rxDATA.DC) + String(",") +
                     String(rxDATA.UV)); */
    if (rxDATA.MODE  == 1) { // Discharging Mode
      led1.setValue(255);
      led2.setValue(255);
    }
    if (rxDATA.MODE  == 2) { // Charging Mode
      blinkLedWidget();
    }
    TimerMillis = millis();
    if (rxDATA.MODE == 3) {
      led1.setValue(0);
      led2.setValue(0);
      if ( TimerMillis - callingTimerMillis >= rxDATA.GSMcallTime * 1000) { // emg Mode
        callingTimerMillis = TimerMillis ;
        modem.callNumber(
          String("+91") +
          String(rxDATA.number[0]) +
          String(rxDATA.number[1]) +
          String(rxDATA.number[2]) +
          String(rxDATA.number[3]) +
          String(rxDATA.number[4]) +
          String(rxDATA.number[5]) +
          String(rxDATA.number[6]) +
          String(rxDATA.number[7]) +
          String(rxDATA.number[8]) +
          String(rxDATA.number[9]));
      }
      // delay(3000);
    }

    if (Mode != rxDATA.MODE) {
      ModeTimer = millis() - ModeTimer;
      WH1 = 0;
      WH2 = 0;
      if (rxDATA.MODE == 1) {
        Blynk.virtualWrite(V18, ModeTimer / 60000);
      }
      if (rxDATA.MODE == 2) {
        Blynk.virtualWrite(V19, ModeTimer / 60000);
      }
      Mode = rxDATA.MODE;
      delay(1000);
      Blynk.run();
    }
    if (Current22 > 0.01 || Current11 > 0.01) {
      if (CounterTimer == 0) {
        lastCapacity1 = WH1;
        lastCapacity2 = WH2;
        CounterTimer = millis();
      }
      if ( rxDATA.MODE == 2) {
        WH1 =((millis() - CounterTimer) * Current11 * (Voltage11 - Voltage22)) / 60000;
        WH2 = (((millis() - CounterTimer) * Current11 * Voltage22)) / 60000;
      }
      if ( rxDATA.MODE == 1) {
        WH1 =((millis() - CounterTimer)  * Current11 * Voltage11) / 60000;
        WH2 = ((millis() - CounterTimer)  * Current22 * Voltage22) / 60000;
      }
      if ((millis() - CounterTimer)  > 30000) {
        if ( rxDATA.MODE == 2) {
          Blynk.virtualWrite(V23, WH1);
          Blynk.virtualWrite(V24, WH2);
          WH1 = 0;
          WH2 = 0;
        }
        if ( rxDATA.MODE == 1) {
          Blynk.virtualWrite(V21, WH1);
          Blynk.virtualWrite(V22, WH2);
          WH1 = 0;
          WH2 = 0;
        }
      CounterTimer = 0;
      }
    } 
    /*Serial.println(String("ATD") +
                   String(rxDATA.number[0]) +
                   String(rxDATA.number[1]) +
                   String(rxDATA.number[2]) +
                   String(rxDATA.number[3]) +
                   String(rxDATA.number[4]) +
                   String(rxDATA.number[5]) +
                   String(rxDATA.number[6]) +
                   String(rxDATA.number[7]) +
                   String(rxDATA.number[8]) +
                   String(rxDATA.number[9]));

    */
    Blynk.run();
  }
