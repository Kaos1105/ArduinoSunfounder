const int LED_PIN_1 = 2;
const int LED_PIN_2 = 3;
const int LED_PIN_3 = 4;
const int LED_PIN_4 = 5;
const int LED_PIN_5 = 6;
const int LED_PIN_6 = 7;
const int LED_PIN_7 = 8;
const int LED_PIN_8 = 9;
const int LED_NUMBER = 9;

int brightness = 0; 
int fadeAmount = 5;

byte LEDPinArray[LED_NUMBER] = { LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4, LED_PIN_5, LED_PIN_6, LED_PIN_7, LED_PIN_8 };

void initAllLEDs() {
  for (int i = 0; i < LED_NUMBER; i++) {
    pinMode(LEDPinArray[i], OUTPUT);
  }
}

void setup() {
  initAllLEDs();
}

void loop() {
  for (int i = 0; i < LED_NUMBER; i++) {
    digitalWrite(LEDPinArray[i], HIGH);
    delay(500);
  }

  for (int i = 0; i < LED_NUMBER; i++) {
    delay(500);
    digitalWrite(LEDPinArray[i], LOW);
  }
}