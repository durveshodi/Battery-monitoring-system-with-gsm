#ifndef Relays_h_
#define Relays_h_

/*----------------*/
#define ON  true
#define OFF false
/*----------------*/
#define relay1 7
#define relay2 A3
#define relay3 5
#define relay4 A1
#define relay5 8
/*-----------------*/
#define smps 10
/*-----------------*/
bool LastStatus = false ;
bool LastStatus1 = false ;
bool LastStatus2 = false ;
bool LastStatus3 = false ;
bool LastStatus4 = false ;
bool LastStatus5 = false ;
bool LastSMPS = false ;


void  SMPS(bool value) {
  LastSMPS = value ;
  digitalWrite(smps , LastSMPS);
}

void Relay1(bool value) {
  LastStatus1 = value ;
  digitalWrite(relay1  , LastStatus1);
}

void Relay2(bool value) {
  LastStatus2 = value ;
  digitalWrite(relay2  , LastStatus2);
}

void Relay3(bool value) {
  LastStatus3 = value ;
  digitalWrite(relay3  , LastStatus3);
}

void Relay4(bool value) {
  LastStatus4 = value ;
  digitalWrite(relay4  , LastStatus4);
}

void DischargeRelay(bool value) {
  LastStatus5 = !value ;
  digitalWrite(relay5  , LastStatus5);
}
void Battery1(bool value) {
  LastStatus1 = value ;
  digitalWrite(relay1  , LastStatus1);
  digitalWrite(relay3  , LastStatus3);
}

void Battery2(bool value) {
  LastStatus2 = value ;
  digitalWrite(relay2  , LastStatus2);
  digitalWrite(relay4  , LastStatus4);
}


void RelayALL(bool value) {
  LastStatus1 = value ;
  LastStatus2 = value ;
  LastStatus3 = value ;
  LastStatus4 = value ;
  LastStatus5 = value ;
  digitalWrite(relay1  , LastStatus1);
  digitalWrite(relay2  , LastStatus2);
  digitalWrite(relay3  , LastStatus3);
  digitalWrite(relay4  , LastStatus4);
  digitalWrite(relay5  , LastStatus5);
  //delay(1000);
}

void  SMPSAndRelaysBegin() {
  pinMode(smps , OUTPUT);
  pinMode(relay1 , OUTPUT);
  pinMode(relay2 , OUTPUT);
  pinMode(relay3 , OUTPUT);
  pinMode(relay4 , OUTPUT);
  pinMode(relay5 , OUTPUT);

}
#endif
