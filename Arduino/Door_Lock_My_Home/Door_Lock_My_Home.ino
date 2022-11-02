#include <Arduino.h>
#include <rdm6300.h>
#include "index.h"
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#define RDM6300_RX_PIN 4  //D2
// #define READ_LED_PIN D1
#define Lock 5     //D1
#define buzzer 16  // D0
#define red 14     //D5
#define green 12   //D6
#define button 13  //D7
WiFiManager wm;
Rdm6300 rdm6300;
#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
int tagRead, sw;
unsigned long MillisGreen = 0, MillisRed = 0;
bool greenStatus = false, redStatus = false;
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Lock, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(red, 1);
  digitalWrite(green, 1);
  //  digitalWrite(blue, 1);
  delay(100);
  digitalWrite(red, 0);
  digitalWrite(green, 0);
  //  digitalWrite(blue, 0);
  wm.setConfigPortalBlocking(false);
  if (wm.autoConnect()) {
    debugln(F("Wifi Connected!"));
    //    setRgbColor(0, 0, 0);//ALL LED LOW
  } else {
    debugln(F("Configportal running"));
    //    setRgbColor(100, 100, 100);//ALL LED HIGH
  }
  rdm6300.begin(RDM6300_RX_PIN);
  debugln("\nPlace RFID tag near the rdm6300...");
  MillisGreen = millis();
  MillisRed = millis();
  
}

void loop() {
  sw = digitalRead(button);
  if (sw == 0) {
    greenled_beep(1);
  }
  RDM();
  millisCheck();
}
void RDM() {
  if (rdm6300.get_new_tag_id()) {
    digitalWrite(LED_BUILTIN, rdm6300.get_tag_id());
    tagRead = rdm6300.get_tag_id(), DEC;
    debugln(tagRead);
    if (tagRead == 13159803 || tagRead == 1206334) {
      buzzer_beep(1);
      greenled_beep(1);
    } else if (tagRead != (10879861) || tagRead != (13159803) || tagRead != (1206334)) {
      buzzer_beep(3);
      redled_beep(3);
    }
  }
}
void buzzer_beep(int times) {
  for (int i = 0; i <= times; i++) {
    Serial.println("buzzer on");
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
  }
}
// void redled_beep(int times) {
//   for (int i = 0; i <= times; i++) {
//     digitalWrite(red, HIGH);
//     delay(100);
//     digitalWrite(red, LOW);
//   }
// }
void redled_beep(int times) {
  Serial.println(redStatus);
  if (redStatus == 0)
    for (int i = 0; i <= times; i++) {
      Serial.println("red led blinking");
      digitalWrite(red, HIGH);
      redStatus = true;
      Serial.println(redStatus);
      MillisRed = millis();
    }
}
void greenled_beep(int times) {
  for (int i = 0; i <= times; i++) {
    Serial.println("green led blinking");
    digitalWrite(green, HIGH);
    digitalWrite(Lock, HIGH);
    greenStatus = true;
    MillisGreen = millis();
  }
}
void millisCheck() {
  if (greenStatus)
    if (millis() - MillisGreen >= 2000) {
      Serial.println("green led off");
      digitalWrite(green, LOW);
      digitalWrite(Lock, LOW);
      greenStatus = false;
    }
  if (redStatus)
    if (millis() - MillisRed >= 100) {
      Serial.println("red led off");
      digitalWrite(red, LOW);
      redStatus = false;
    }
}