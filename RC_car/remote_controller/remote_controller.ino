                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    #define LEFT_J 0
#define RIGHT_J 1
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

void setup()
{
  Serial.begin(9600);
  mySerial.begin(115200);

  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

int buf[10];

int* Preparing_Values_To_Send(int servo, int motor, int* result)
 {
  result[0] = 0x7E;
  result[4] = servo >> 24;
  result[3] = servo >> 16;
  result[2] = servo >> 8;
  result[1] = servo % 256;

  result[8] = motor >> 24;
  result[7] = motor >> 16;
  result[6] = motor >> 8;
  result[5] = motor % 256;
  result[9] = 0x8A;
  return result;
 }


void loop()
{
  int Servo_data = analogRead(LEFT_J);
  int Motor_data = analogRead(RIGHT_J);
  Serial.print('[');
  Serial.print(Servo_data);
  Serial.print(',');
  Serial.print(Motor_data);
  Serial.print(']');
  Preparing_Values_To_Send(Servo_data, Motor_data, buf);

  for (int i=0; i < 10; i++)
 {
  mySerial.write(buf[i]);
 }

}
