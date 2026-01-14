#ifndef ROBOTMANAGER_H
#define ROBOTMANAGER_H

#include "../globals.h"
#include <ESP32Servo.h>
#include <Preferences.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SERVO1_PIN 14
#define SERVO2_PIN 27
#define SERVO3_PIN 26
#define SERVO4_PIN 25
#define SERVO5_PIN 33
#define SERVO6_PIN 32

#define SERVO_1_OFFSET 56
#define SERVO_2_OFFSET 40
#define SERVO_3_OFFSET 14
#define SERVO_4_OFFSET 138
#define SERVO_5_OFFSET 80
#define SERVO_6_OFFSET 75

#define DEFAULT_SPEED 50
#define MOVEMENT_DELAY_MAX 100
#define MOVEMENT_DELAY_MIN 5

class RobotManager
{
public:
  RobotManager();
  void init();
  void setServoTargetPosition(int servo, int position);
  void setServoOffset(int servo, int offset);
  void setPlacePosition(int servo, int angle);
  void setMovementSpeed(int speed);

  int getServoPosition(int servo);
  int getServoOffset(int servo);
  int getPlacePosition(int servo);
  int getServoPositionWithOffset(int servo);
  int getMovementSpeed();

private:
  static void moveServoTask(void* parameters);
  void        startServoTasks();

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

  int place1 = 0;
  int place2 = 0;
  int place3 = 0;
  int place4 = 0;
  int place5 = 0;
  int place6 = 0;

  int _movementSpeed     = DEFAULT_SPEED;
  int _targetPosition[6] = {0, 0, 0, 0, 0, 0};

  typedef struct
  {
    int           servo_id;
    RobotManager* robotManager;
  } ServoTaskParams_t;

  ServoTaskParams_t _taskParams[6];
  TaskHandle_t      _servoTaskHandles[6];
};

#endif // ROBOTMANAGER_H
