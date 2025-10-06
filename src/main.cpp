/**
 * @file main.cpp
 * @author InbBiot Monitoring
 * @brief Main entry point for the InbBiot Monitoring quality test application
 * @version 0.1
 * @date 2025-05-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <RobotArm.h>

RobotArm robotArm;

void setup()
{
  Serial.begin(115200);
  delay(500);
  robotArm.startRobotArm();
}

void loop()
{
  vTaskDelete(NULL);
}
