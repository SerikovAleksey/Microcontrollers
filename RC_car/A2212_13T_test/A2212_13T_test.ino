#include <Servo.h>

Servo motor;

void setup() {
pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  motor.attach(4);
  motor.writeMicroseconds(2300);
  delay(2000);
  motor.writeMicroseconds(800);
  delay(6000);
}


void loop() {
int val = map(analogRead(1), 0, 1023, 800, 2300);
motor.writeMicroseconds(val);
}
