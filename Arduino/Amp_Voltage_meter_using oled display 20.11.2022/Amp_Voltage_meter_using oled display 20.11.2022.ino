/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define ANALOG_IN_PIN A0

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0;  // resistance of R1 (100K) -see text!
float R2 = 10000.0;   // resistance of R2 (10K) - see text!
int value = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("DC VOLT & AMP METER");
  display.display();
  delay(100);
}

void loop() {
  // Scroll();
  Voltage();
}
void Voltage() {
  value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0;  // see text
  vin = vout / (R2 / (R1 + R2));
  if (vin < 0.09) {
    vin = 0.0;  //statement to quash undesired reading !
  }
  // lcd.setCursor(0, 1);
  Serial.print("INPUT V= ");
  Serial.println(vin);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("DC VOLT & AMP METER");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  // Display static text
  display.print("Voltage: ");
  display.println(vin);
  display.display();
  delay(100);
  delay(500);
}
void Scroll() {

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}