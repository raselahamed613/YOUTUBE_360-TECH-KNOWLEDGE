const int ledPin = 13;
int ledState = LOW;             // ledState used to set the LED
int flash = 10;                 // Flash LED 5 times--two passes for ON/OFF sequence

unsigned long previousMillis = 0;        // will store last time LED was updated

const long interval = 500;           // interval at which to blink (milliseconds)


void setup() {
  pinMode (ledPin, OUTPUT);
  previousMillis = millis();
}

void loop() {

  unsigned long currentMillis = millis();
  for (int i = 0; i < flash ; i++) {
    while ( (currentMillis - previousMillis) < interval) {
      currentMillis = millis();
    }
    previousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
  ledState = LOW;
  digitalWrite(ledPin, ledState);

  delay (2000);
}