int xPin = A0;
int yPin = A1;
int buttonPin = 7;

int upLED = 10;
int downLED = 6;
int leftLED = 11;
int rightLED = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(upLED, OUTPUT);
  pinMode(downLED, OUTPUT);
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  bool btnPressed = digitalRead(buttonPin);

  int upPWM = map(yVal, 506, 0, 0, 255);
  int downPWM = map(yVal, 506, 1023, 0, 255);
  int leftPWM = map(xVal, 516, 0, 0, 255);
  int rightPWM = map(xVal, 516, 1023, 0, 255);

  if (yVal <= 506) {
    analogWrite(upLED, upPWM);
  }

  if (yVal >= 506) {
    analogWrite(downLED, downPWM);
  }

  if (xVal <= 516) {
    analogWrite(leftLED, leftPWM);
  }

  if (xVal >= 516) {
    analogWrite(rightLED, rightPWM);
  }

  if (btnPressed == LOW) {
    digitalWrite(upLED, HIGH);
    digitalWrite(downLED, HIGH);
    digitalWrite(leftLED, HIGH);
    digitalWrite(rightLED, HIGH);
  }
  // Serial.print("X = ");
  // Serial.print(xVal);

  // Serial.println("   Y = ");
  // Serial.println(yVal);

  Serial.println("pressed");
  Serial.println(btnPressed);

  delay(100);
}
