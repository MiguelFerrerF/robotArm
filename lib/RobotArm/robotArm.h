
#ifndef ROBOTARM_H
#define ROBOTARM_H

#include "RobotManager/robotManager.h"
#include "globals.h"
#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#define READ_BUFFER_SIZE 200

class RobotArm
{
public:
  RobotArm();
  ~RobotArm();

  void startRobotArm();

private:
  static void serialTask(void* pvParameters);
  static void processCommandTask(void* pvParameters);

  void setupCommand(const char* param, const char* value);
  void placeCommand(const char* q1, const char* q2, const char* q3, const char* q4, const char* q5, const char* q6);
  void readCommand(const char* variable);

  TaskHandle_t  _serialTaskHandle = nullptr;
  QueueHandle_t _commandQueue     = nullptr;

  bool _isRunning      = false;
  bool _abortRequested = false;

  RobotManager robotManager;
};

#endif // ROBOTARM_H
