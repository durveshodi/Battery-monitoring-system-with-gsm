#include <SoftwareSerial.h>
#include <SoftEasyTransfer.h>

SoftwareSerial mySerial(2, 3);


SoftEasyTransfer ETin, ETout;

struct RECEIVE_DATA_STRUCTURE {
  float receivedDATA;
};

struct SEND_DATA_STRUCTURE {
  int number[10];
  float Voltage1;
  float Voltage2;
  float Current1;
  float Current2;
  float Temperature1;
  float Temperature2;
};

RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;

void setup() {
  mySerial.begin(9600);
  ETin.begin(details(rxdata), &mySerial);
  ETout.begin(details(txdata), &mySerial);

}
void SendDATA() {
  
  txdata.number[0] = 2;
  txdata.number[1] = 4;
  txdata.number[2] = 0;
  txdata.number[3] = 3;
  txdata.number[4] = 4;
  txdata.number[5] = 4;
  txdata.number[6] = 5;
  txdata.number[7] = 7;
  txdata.number[8] = 9;
  txdata.number[9] = 9;
  txdata.number[10] = 0;
  
  txdata.Voltage1 = 0.10;
  txdata.Voltage2 = 1.11;
  txdata.Current1 = 15.2;
  txdata.Current2 = 1.13;
  txdata.Temperature1 = 16.4;
  txdata.Temperature2 = 18.5;
  ETout.sendData();
}

void ReceiveDATA() {
  ETin.receiveData();
  if (rxdata.receivedDATA == false) {
    rxdata.receivedDATA = true;
  } else {
    rxdata.receivedDATA = false;
  }
  digitalWrite(13, rxdata.receivedDATA);
}
void loop() {
  SendDATA();
  ReceiveDATA();
  delay(1000);
}
