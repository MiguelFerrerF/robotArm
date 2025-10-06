
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
  void moveCommand(const char* x, const char* y, const char* z);
  void readCommand(const char* variable);

  TaskHandle_t  _serialTaskHandle = nullptr;
  QueueHandle_t _commandQueue     = nullptr;

  RobotManager robotManager;
};

#endif // ROBOTARM_H
