int buttonPin = 7;
int ledPin = 11;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rawValue = analogRead(A0);

  float voltage = rawValue * (5.0 / 1023.0);

  Serial.print(voltage);
  Serial.println("Volts");

  delay(100);
}
