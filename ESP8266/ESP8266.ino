#include <SoftwareSerial.h>
SoftwareSerial espSerial(13, 11); //Rx,Tx

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    espSerial.begin(9600);
}

void loop() {
    if (espSerial.available()) {
        Serial.write(espSerial.read());
    }
    if (Serial.available()) {
        espSerial.write(Serial.read());
    }
}