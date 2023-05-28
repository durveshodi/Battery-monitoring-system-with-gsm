#include <SoftwareSerial.h>
#include <SoftEasyTransfer.h>

SoftwareSerial mySerial(2, 3);


SoftEasyTransfer ETin, ETout;

struct RECEIVE_DATA_STRUCTURE {
  int number[10];
  float Voltage1;
  float Voltage2;
  float Current1;
  float Current2;
  float Temperature1;
  float Temperature2;
};

struct SEND_DATA_STRUCTURE {
  float receivedDATA;
};

RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600); 
  ETin.begin(details(rxdata), &mySerial);
  ETout.begin(details(txdata), &mySerial);

}
void SendDATA() {
  
  if(ETin.receiveData()){
  txdata.receivedDATA = true;
  }else{
  txdata.receivedDATA = false;
    }
  ETout.sendData();
}

void ReceiveDATA() {
  ETin.receiveData();
  rxdata.number[0];
  rxdata.number[1];
  rxdata.number[2];
  rxdata.number[3];
  rxdata.number[4];
  rxdata.number[5];
  rxdata.number[6];
  rxdata.number[7];
  rxdata.number[8];
  rxdata.number[9];
  
  rxdata.Voltage1 ;
  rxdata.Voltage2 ;
  rxdata.Current1 ;
  rxdata.Current2;
  rxdata.Temperature1;
  rxdata.Temperature2;
}
void loop() {
  SendDATA();
  ReceiveDATA();
  Serial.println(String(rxdata.number[0]) + String(rxdata.number[1]) +  String(rxdata.number[2]) + String(rxdata.number[3]) + String(rxdata.number[4]) + String(rxdata.number[5]) + String(rxdata.number[6]) + String(rxdata.number[7]) + String(rxdata.number[8]) + String(rxdata.number[9]) );
  Serial.println(rxdata.Voltage1);
  Serial.println(rxdata.Voltage2);
  Serial.println(rxdata.Current1);
  Serial.println(rxdata.Current2);
  Serial.println(rxdata.Temperature1);
  Serial.println(rxdata.Temperature2);
  delay(1000);
}
