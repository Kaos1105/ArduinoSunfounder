// int buzzerPin = 8;
// int buttonPin = 7;

// void setup() {
//   pinMode(buzzerPin, OUTPUT);
//   pinMode(buttonPin, INPUT_PULLUP);
// }

// void loop() {
//   int buttonState = digitalRead(buttonPin);

//   if (buttonState == LOW) {
//     digitalWrite(buzzerPin, HIGH);
//   }

//   if (buttonState == HIGH) {
//     digitalWrite(buzzerPin, LOW);
//   }
// }

int buzzerPin = 8;

void setup() {
  pinMode(buzzerPin, OUTPUT);

  tone(buzzerPin, 1000, 2000);
}

void loop() {
  tone(buzzerPin, 440);  // A4
  delay(1000);

  tone(buzzerPin, 494);  // B4
  delay(1000);

  tone(buzzerPin, 523);  // C4
  delay(1000);

  tone(buzzerPin, 587);  // D4
  delay(1000);

  tone(buzzerPin, 659);  // E4
  delay(1000);

  tone(buzzerPin, 698);  // F4
  delay(1000);

  tone(buzzerPin, 784);  // G4
  delay(1000);

  noTone(buzzerPin);
  delay(1000);
}