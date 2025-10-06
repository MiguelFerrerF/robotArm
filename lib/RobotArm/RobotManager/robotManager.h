#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H

#include <ESP32Servo.h>

#define SERVO1_PIN 14
#define SERVO2_PIN 27
#define SERVO3_PIN 26
#define SERVO4_PIN 25
#define SERVO5_PIN 33
#define SERVO6_PIN 32

class RobotManager
{
public:
  RobotManager();
  void init();
  void setServoPosition(int servo, int position);
  int  getServoPosition(int servo);

private:
  Servo servo1;
  Servo servo2;
  Servo servo3;
  Servo servo4;
  Servo servo5;
  Servo servo6;
};

#endif // ROBOTMANAGER_H
