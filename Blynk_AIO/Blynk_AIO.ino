#include <IRremote.h>
#include <EEPROM.h>

#define BLYNK_TEMPLATE_ID "TMPL6rEgFiyph"
#define BLYNK_TEMPLATE_NAME "Sunfounder 3in1"
#define BLYNK_AUTH_TOKEN "71WrZvvzU8RaGEeaYpX5sBk1h8Q-etue"
#define BLYNK_PRINT Serial
#define ESP8266_BAUD 9600

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dreamplex";
char pass[] = "Dreamchaser";

const int IR_RECEIVE_PIN = 12;  // Define the pin number for the IR Sensor

SoftwareSerial EspSerial(13, 11);  // RX, TX

// Your ESP8266 baud rate:
ESP8266 wifi(&EspSerial);

const int A_1B = 5;
const int A_1A = 6;
const int B_1B = 9;
const int B_1A = 10;

const int echoPin = 4;
const int trigPin = 3;

const int rightIR = 7;
const int leftIR = 8;

const int lineTrackPin = 2;

int speed = 150;
String flag = "NONE";

float leftOffset = 1.0;
float rightOffset = 1.0;

int Xvalue = 0;
int Yvalue = 0;

BLYNK_WRITE(V9) {
  int value = param.asInt();
  Serial.println(value);
  Xvalue = value;
}

BLYNK_WRITE(V10) {
  int value = param.asInt();
  Serial.println(value);
  Yvalue = value;
}

void setup() {
  Serial.begin(9600);

  // Blynk setup
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);

  //motor
  pinMode(A_1B, OUTPUT);
  pinMode(A_1A, OUTPUT);
  pinMode(B_1B, OUTPUT);
  pinMode(B_1A, OUTPUT);

  //ultrasonic
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  //IR obstacle
  pinMode(leftIR, INPUT);
  pinMode(rightIR, INPUT);

  //Line Track Module
  pinMode(lineTrackPin, INPUT);

  //IR remote
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  // Start the IR receiver // Start the receiver
  Serial.println("REMOTE CONTROL START");

  EEPROM.write(0, 98);                  //write the offset to the left motor
  EEPROM.write(1, 100);                 //write the offset to the right motor
  leftOffset = EEPROM.read(0) * 0.01;   //read the offset
  rightOffset = EEPROM.read(1) * 0.01;  //read the offset
}


void blynkControl() {
  //Blynk section
  Blynk.run();
  if (!Blynk.connected()) {
    // digitalWrite(ledPin, LOW);
    Serial.print("offline!");
    bool result = Blynk.connect();
    Serial.println(result);
  } else {
    // digitalWrite(ledPin, HIGH);
    // Serial.print("online");
  }

  if (Yvalue >= 5) {
    moveForward(255);
  } else if (Yvalue <= -5) {
    moveBackward(255);
  } else if (Xvalue >= 5) {
    turnRight(150);
  } else if (Xvalue <= -5) {
    turnLeft(150);
  } else {
    stopMove();
  }
}

void loop() {

  // blynkControl();

  // IRHandler();

  // driveMode();
   Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  if (!Blynk.connected()) {
    // digitalWrite(ledPin, LOW);
    Serial.print("offline!");
    bool result = Blynk.connect();
    Serial.println(result);
  } else {
    // digitalWrite(ledPin, HIGH);
    // Serial.print("online");
  }

  if (Yvalue >= 5) {
    moveForward(255);
  } else if (Yvalue <= -5) {
    moveBackward(255);
  } else if (Xvalue >= 5) {
    turnRight(150);
  } else if (Xvalue <= -5) {
    turnLeft(150);
  } else {
    stopMove();
  }
}

void IRHandler() {
  if (IrReceiver.decode()) {
    //    Serial.println(results.value,HEX);
    String key = decodeKeyValue(IrReceiver.decodedIRData.command);
    if (key != "ERROR") {
      Serial.println(key);

      if (key == "+") {
        speed += 50;
        Serial.println(speed);
      } else if (key == "-") {
        speed -= 50;
        Serial.println(speed);
      } else if (key == "2") {
        moveForward(speed);
        delay(1000);
      } else if (key == "1") {
        moveLeft(speed);
      } else if (key == "3") {
        moveRight(speed);
      } else if (key == "4") {
        turnLeft(speed);
      } else if (key == "6") {
        turnRight(speed);
      } else if (key == "7") {
        backLeft(speed);
      } else if (key == "9") {
        backRight(speed);
      } else if (key == "8") {
        moveBackward(speed);
        delay(1000);
      } else if (key == "CYCLE") {
        flag = "LINE";
      } else if (key == "U/SD") {
        flag = "AUTO";
      } else if (key == "0") {
        flag = "NONE";
        stopMove();
      } else if (key == "FORWARD") {
        flag = "ULTR";
      } else if (key == "BACKWARD") {
        flag = "IROB";
      } else if (key == "EQ") {
        flag = "FOLW";
      }

      if (speed >= 255) {
        speed = 255;
      }
      if (speed <= 0) {
        speed = 0;
      }
      delay(500);
      stopMove();
    }

    IrReceiver.resume();  // Enable receiving of the next value
  }
}

void driveMode() {
  if (flag == "AUTO") {
    autoDrive(speed);
  } else if (flag == "LINE") {
    lineTrack(speed);
  } else if (flag == "ULTR") {
    ultrasonicExample(speed);
  } else if (flag == "IROB") {
    irobstacleExample(speed);
  } else if (flag == "FOLW") {
    following(speed);
  }
}


float readSensorData() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float distance = pulseIn(echoPin, HIGH) / 58.00;  //Equivalent to (340m/s*1us)/2
  return distance;
}

void moveForward(int speed) {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, int(speed * rightOffset));
  analogWrite(B_1B, int(speed * leftOffset));
  analogWrite(B_1A, 0);
}

void moveBackward(int speed) {
  analogWrite(A_1B, speed);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, speed);
}

void turnRight(int speed) {
  analogWrite(A_1B, speed);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, speed);
  analogWrite(B_1A, 0);
}

void turnLeft(int speed) {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, speed);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, speed);
}

void moveLeft(int speed) {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, speed);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, 0);
}

void moveRight(int speed) {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, speed);
  analogWrite(B_1A, 0);
}

void backLeft(int speed) {
  analogWrite(A_1B, speed);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, 0);
}

void backRight(int speed) {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, int(speed));
}

void stopMove() {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, 0);
}

void autoDrive(int speed) {
  int left = digitalRead(leftIR);  // 0: Obstructed   1: Empty
  int right = digitalRead(rightIR);

  if (!left && right) {
    backLeft(speed);
  } else if (left && !right) {
    backRight(speed);
  } else if (!left && !right) {
    moveBackward(speed);
  } else {
    float distance = readSensorData();
    Serial.println(distance);
    if (distance > 50) {  // Safe
      moveForward(200);
    } else if (distance < 10 && distance > 2) {  // Attention
      moveBackward(200);
      delay(1000);
      backLeft(150);
      delay(500);
    } else {
      moveForward(150);
    }
  }
}

void following(int speed) {
  float distance = readSensorData();

  int left = digitalRead(leftIR);  // 0: Obstructed   1: Empty
  int right = digitalRead(rightIR);

  if (distance > 5 && distance < 10) {
    moveForward(speed);
  }
  if (!left && right) {
    turnLeft(speed);
  } else if (left && !right) {
    turnRight(speed);
  } else {
    stopMove();
  }
}

void lineTrack(int speed) {
  int lineColor = digitalRead(lineTrackPin);  // 0:white  1:black
  Serial.println(lineColor);
  if (lineColor) {
    moveLeft(speed);
  } else {
    moveRight(speed);
  }
}

void irobstacleExample(int speed) {
  int left = digitalRead(leftIR);  // 0: Obstructed   1: Empty
  int right = digitalRead(rightIR);

  if (!left && right) {
    backLeft(speed);
  } else if (left && !right) {
    backRight(speed);
  } else if (!left && !right) {
    moveBackward(speed);
  } else {
    stopMove();
  }
}

void ultrasonicExample(int speed) {
  float distance = readSensorData();
  Serial.println(distance);
  if (distance > 25) {
    moveForward(speed);
  } else if (distance < 10 && distance > 2) {
    moveBackward(speed);
  } else {
    stopMove();
  }
}

String decodeKeyValue(long result) {
  switch (result) {
    case 0x16:
      return "0";
    case 0xC:
      return "1";
    case 0x18:
      return "2";
    case 0x5E:
      return "3";
    case 0x8:
      return "4";
    case 0x1C:
      return "5";
    case 0x5A:
      return "6";
    case 0x42:
      return "7";
    case 0x52:
      return "8";
    case 0x4A:
      return "9";
    case 0x9:
      return "+";
    case 0x15:
      return "-";
    case 0x7:
      return "EQ";
    case 0xD:
      return "U/SD";
    case 0x19:
      return "CYCLE";
    case 0x44:
      return "PLAY/PAUSE";
    case 0x43:
      return "FORWARD";
    case 0x40:
      return "BACKWARD";
    case 0x45:
      return "POWER";
    case 0x47:
      return "MUTE";
    case 0x46:
      return "MODE";
    case 0x0:
      return "ERROR";
    default:
      return "ERROR";
  }
}