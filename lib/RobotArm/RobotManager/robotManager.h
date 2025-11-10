#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H

#include <ESP32Servo.h>
#include <Preferences.h>

#define SERVO1_PIN 14
#define SERVO2_PIN 27
#define SERVO3_PIN 26
#define SERVO4_PIN 25
#define SERVO5_PIN 33
#define SERVO6_PIN 32

#define SERVO_1_OFFSET 24
#define SERVO_2_OFFSET 103
#define SERVO_3_OFFSET 20
#define SERVO_4_OFFSET 148
#define SERVO_5_OFFSET 82
#define SERVO_6_OFFSET 0

class RobotManager
{
public:
  RobotManager();
  void init();
  void setServoPosition(int servo, int position);
  void setServoOffset(int servo, int offset);

  int getServoPosition(int servo);
  int getServoOffset(int servo);
  int getServoPositionWithOffset(int servo);

private:
  Preferences preferences;

  Servo servo1;
  Servo servo2;
  Servo servo3;
  Servo servo4;
  Servo servo5;
  Servo servo6;

  int offset1 = SERVO_1_OFFSET;
  int offset2 = SERVO_2_OFFSET;
  int offset3 = SERVO_3_OFFSET;
  int offset4 = SERVO_4_OFFSET;
  int offset5 = SERVO_5_OFFSET;
  int offset6 = SERVO_6_OFFSET;
};

#endif // ROBOTMANAGER_H
