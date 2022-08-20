#include <Arduino.h>
#include <rdm6300.h>
#define RDM6300_RX_PIN D2
// #define READ_LED_PIN D1
#define red 14    //D5
#define green 12  //D6
#define blue 13   //D7

Rdm6300 rdm6300;
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
  rdm6300.begin(RDM6300_RX_PIN);
  Serial.println("\nPlace RFID tag near the rdm6300...");
}

void loop() {
  RDM();
}
void RDM() {
  if (rdm6300.get_new_tag_id()) {
    Serial.println(rdm6300.get_tag_id(), DEC);
    digitalWrite(LED_BUILTIN, rdm6300.get_tag_id());
    tagRead = rdm6300.get_tag_id();
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