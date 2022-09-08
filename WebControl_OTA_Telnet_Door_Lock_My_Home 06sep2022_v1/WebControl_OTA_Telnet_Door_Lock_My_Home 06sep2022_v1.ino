#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <rdm6300.h>
#include "index.h"
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>
#define RDM6300_RX_PIN 4  //D2
// #define READ_LED_PIN D1
#define Lock 5      //D1
#define buzzer 16   // D0
#define red 14      //D5
#define green 12    //D6
#define button1 13  //D7
#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
int tagRead, sw, gpio4Value;
unsigned long MillisGreen = 0, MillisRed = 0;
bool greenStatus = false, redStatus = false;
const char *ota_id = "neways", *ota_password = "neways12345";
String Chipid = "";
WiFiManager wm;
Rdm6300 rdm6300;
ESP8266WebServer server(80);

WiFiServer TelnetServer(23);
WiFiClient Telnet;
void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Lock, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button1, INPUT);
  digitalWrite(red, 1);
  digitalWrite(green, 1);
  digitalWrite(LED_BUILTIN, HIGH);
  //  digitalWrite(blue, 1);
  delay(100);
  digitalWrite(red, 0);
  digitalWrite(green, 0);
  //  digitalWrite(blue, 0);

  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(60);
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

  // server.on("/", []() {
  //   server.send(200, "text/plain", "Hi! I am ESP8266.");
  // });
  server.on("/", handleRoot);
  server.on("/product", machine_control);
  // espServer.begin();          /* Start the HTTP web Server */
  ElegantOTA.begin(&server, ota_id, ota_password);  // Start ElegantOTA
  server.begin();

  Serial.print(F(__FILE__));  // Always display sketch name and date info
  Serial.print(F("\t |"));
  Serial.print(F(__DATE__));
  Serial.print(F("\t |"));
  Serial.println(F(__TIME__));
  Serial.print(F("ChipId: "));
  Serial.println(Chipid);
  Serial.printf("ChipID: %08X\n", ESP.getFlashChipId());

  Serial.printf("Default hostname: %s\n", WiFi.hostname().c_str());
  TelnetServer.begin();
  TelnetServer.setNoDelay(true);
}

void loop() {
  sw = digitalRead(button1);
  if (sw == 0) {
    greenled_beep(1);
  }
  RDM();
  server.handleClient();
  wm.process();
  millisCheck();
  handleTelnet();
  // Web();
}
void RDM() {
  if (rdm6300.get_new_tag_id()) {
    digitalWrite(LED_BUILTIN, rdm6300.get_tag_id());
    tagRead = rdm6300.get_tag_id(), DEC;
    debugln(tagRead);
    Telnet.println(tagRead);
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


// void Web() {
//   WiFiClient client = espServer.available(); /* Check if a client is available */
//   if (!client) {
//     return;
//   }

//   Serial.println("New Client!!!");

//   String request = client.readStringUntil('\r'); /* Read the first line of the request from client */
//   Serial.println(request);                       /* Print the request on the Serial monitor */
//   /* The request is in the form of HTTP GET Method */
//   client.flush();

//   /* Extract the URL of the request */
//   /* We have four URLs. If IP Address is 192.168.1.6 (for example),
//    * then URLs are:
//    * 192.168.1.6/GPIO4ON and its request is GET /GPIO4ON HTTP/1.1
//    * 192.168.1.6/GPIO4OFF and its request is GET /GPIO4OFF HTTP/1.1
//    * 192.168.1.6/GPIO5ON and its request is GET /GPIO5ON HTTP/1.1
//    * 192.168.1.6/GPIO4OFF and its request is GET /GPIO5OFF HTTP/1.1
//    */
//   /* Based on the URL from the request, turn the LEDs ON or OFF */
//   if (request.indexOf("/GPIO4ON") != -1) {
//     Serial.println("GPIO4 LED is ON");
//     // digitalWrite(gpio4LEDPin, HIGH);
//     gpio4Value = HIGH;
//     digitalWrite(LED_BUILTIN, LOW);
//   }
//   if (request.indexOf("/GPIO4OFF") != -1) {
//     Serial.println("GPIO4 LED is OFF");
//     // digitalWrite(gpio4LEDPin, LOW);
//     digitalWrite(LED_BUILTIN, HIGH);
//     gpio4Value = LOW;
//   }


//   /* HTTP Response in the form of HTML Web Page */
//   client.println("HTTP/1.1 200 OK");
//   client.println("Content-Type: text/html");
//   client.println();  //  IMPORTANT
//   client.println("<!DOCTYPE HTML>");
//   client.println("<html>");
//   client.println("<head>");
//   client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//   client.println("<link rel=\"icon\" href=\"data:,\">");
//   /* CSS Styling for Buttons and Web Page */
//   client.println("<style>");
//   client.println("html { font-family: Courier New; display: inline-block; margin: 0px auto; text-align: center;}");
//   client.println(".button {border: none; color: white; padding: 10px 20px; text-align: center;");
//   client.println("text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer;}");
//   client.println(".button1 {background-color: #13B3F0;}");
//   client.println(".button2 {background-color: #3342FF;}");
//   client.println("</style>");
//   client.println("</head>");

//   /* The main body of the Web Page */
//   client.println("<body>");
//   client.println("<h2>ESP8266 Web Server</h2>");

//   if (gpio4Value == LOW) {
//     client.println("<p>GPIO4 LED Status: OFF</p>");
//     client.print("<p><a href=\"/GPIO4ON\"><button class=\"button button1\">Click to turn ON</button></a></p>");
//   } else {
//     client.println("<p>GPIO4 LED Status: ON</p>");
//     client.print("<p><a href=\"/GPIO4OFF\"><button class=\"button button2\">Click to turn OFF</button></a></p>");
//   }

//   client.println("</body>");
//   client.println("</html>");
//   client.print("\n");

//   delay(1);
//   /* Close the connection */
//   client.stop();
//   Serial.println("Client disconnected");
//   Serial.print("\n");
// }
void handleRoot() {
  String s = webpage;
  server.send(200, "text/html", s);
}

void machine_control() {
  String state = "OFF";
  String act_state = server.arg("state");
  if (act_state == "1") {
    //    user_allow_status();
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    state = "OFF";
  } else if (act_state == "0") {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    state = "ON";
  }
  server.send(200, "text/plane", state);
}
void handleTelnet() {
  if (TelnetServer.hasClient()) {
    if (!Telnet || !Telnet.connected()) {
      if (Telnet) Telnet.stop();
      Telnet = TelnetServer.available();
      Telnet.flush();
    } else {
      TelnetServer.available().stop();
    }
  }
}