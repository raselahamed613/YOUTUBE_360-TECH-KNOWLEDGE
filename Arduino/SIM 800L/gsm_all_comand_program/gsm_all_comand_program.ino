#include <SoftwareSerial.h>
SoftwareSerial gsm(3, 2);
String msg;
char call;

void setup() {
  gsm.begin(9600);
  Serial.begin(9600);
  Serial.println("key to take an action:");
  Serial.println("a : to hangup a call");
  Serial.println("r : to receive a call");
  Serial.println("s : to send message");
  Serial.println("c : to make a call");
  Serial.println("e : to redial");
  Serial.println("g : to gps location");
}

void loop() {
  if (Serial.available() > 0)
    switch (Serial.read()) {
      case 's':
        SendMessage();
        break;
      case 'c':
        MakeCall();
        break;
      case 'a':
        HangupCall();
        break;
      case 'e':
        RedialCall();
        break;
      case 'r':
        ReceiveCall();
        break;
      case 'g':
        Gps();
        break;
    }
  if (gsm.available() > 0)
    Serial.write(gsm.read());
}

void SendMessage() {
  gsm.println("AT+CMGF=1");
  delay(1000);
  // gsm.println("AT+CMGS=\"+01684726517\"\r");
  gsm.println("AT+CMGS=\"+8801684726517\"");
  delay(1000);
  gsm.println("hello ji ! how are you");
  delay(100);
  gsm.println((char)26);
  delay(1000);
}

void MakeCall() {
  gsm.println("ATD+ +8801684726517;");
  Serial.println("Calling  ");
  delay(1000);
}
void Gps() {
  gsm.println("AT+CGATT=1");
  delay(1000);
  gsm.println("AT+SAPBR=3,1," CONTYPE "," GPRS "");
  delay(1000);
  gsm.println("AT+SAPBR=3,1," APN "," RCMNET "");
  delay(1000);
  gsm.println("AT+SAPBR=1,1");
  delay(1000);
  gsm.println("AT+SAPBR=2,1");
  delay(1000);
  gsm.println("AT+CIPGSMLOC=1,1");
}


void HangupCall() {
  gsm.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall() {
  gsm.println("ATA");
  delay(1000);
  {
    call = gsm.read();
    Serial.print(call);
  }
}

void RedialCall() {
  gsm.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}

void ReceiveMessage() {
  gsm.println("AT+CMGF=1");
  delay(1000);
  if (gsm.available() > 0) {
    gsm.println("AT+CNMI=2,2,0,0,0");
    delay(1000);
    Serial.print(gsm.read());
  }
}