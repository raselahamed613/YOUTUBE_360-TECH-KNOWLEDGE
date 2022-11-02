#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2);  //SIM800L Tx & Rx is connected to Arduino #3 & #2
#define testPin 10
void setup() {
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  pinMode(testPin, INPUT_PULLUP);
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);
  updateSerial();
}

void loop() {
  int val = digitalRead(testPin);
  // Serial.println(val);
  if (val == 0) {
    call();
    Serial.println("Calling...");
  }
  updateSerial();
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());  //Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());  //Forward what Software Serial received to Serial Port
  }
}
void call() {
  mySerial.println("AT");  //Once the handshake test is successful, i t will back to OK
  updateSerial();
  mySerial.println("ATD+ +8801684726517;");  //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  updateSerial();
  delay(20000);             // wait for 20 seconds...
  mySerial.println("ATH");  //hang up
  updateSerial();
}