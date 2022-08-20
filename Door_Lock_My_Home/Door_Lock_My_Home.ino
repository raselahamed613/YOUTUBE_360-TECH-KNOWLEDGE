#include <Arduino.h>
#include <rdm6300.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#define RDM6300_RX_PIN D2
// #define READ_LED_PIN D1
#define red 14    //D5
#define green 12  //D6
#define blue 13   //D7
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
int tagRead;
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, 1);
  digitalWrite(green, 1);
  digitalWrite(blue, 1);
  delay(100);
  digitalWrite(red, 0);
  digitalWrite(green, 0);
  digitalWrite(blue, 0);
  wm.setConfigPortalBlocking(false);
  if (wm.autoConnect()) {
    debugln(F("Wifi Connected!"));
    //    setRgbColor(0, 0, 0);//ALL LED LOW
  }  else {
    debugln(F("Configportal running"));
    //    setRgbColor(100, 100, 100);//ALL LED HIGH
  }
  rdm6300.begin(RDM6300_RX_PIN);
  debugln("\nPlace RFID tag near the rdm6300...");
}

void loop() {
  RDM();
}
void RDM() {
  if (rdm6300.get_new_tag_id()) {
    digitalWrite(LED_BUILTIN, rdm6300.get_tag_id());
    tagRead = rdm6300.get_tag_id(), DEC;
    debugln(tagRead);
    if (tagRead == 10879861) {
      digitalWrite(green, 1);
      delay(1000);
      digitalWrite(green, 0);
    } else if (tagRead != (10879861)) {
      digitalWrite(red, 1);
      delay(300);
      digitalWrite(red, 0);
    }
  }
}
