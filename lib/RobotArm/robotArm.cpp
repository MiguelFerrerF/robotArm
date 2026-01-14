
#include "robotArm.h"
#include <ctype.h>

RobotArm::RobotArm()
{
  _serialTaskHandle = nullptr;
  _commandQueue     = nullptr;
}

RobotArm::~RobotArm()
{
}

void RobotArm::startRobotArm()
{
  if (_serialTaskHandle != nullptr) {
    Serial.println("[ERROR] RobotArm already running.");
    return;
  }

  _commandQueue = xQueueCreate(10, sizeof(char[50])); // Crear cola para comandos
  if (_commandQueue == nullptr) {
    Serial.println("[ERROR] Failed to create command queue.");
    return;
  }

  xTaskCreatePinnedToCore(serialTask,         // Tarea para leer comandos
                          "SerialTask",       // Nombre de la tarea
                          4096,               // Tamaño de pila
                          this,               // Parámetro de la tarea
                          2,                  // Prioridad alta
                          &_serialTaskHandle, // Manejador
                          1                   // Núcleo
  );

  xTaskCreatePinnedToCore(processCommandTask,   // Tarea para procesar comandos
                          "ProcessCommandTask", // Nombre de la tarea
                          4096,                 // Tamaño de pila
                          this,                 // Parámetro de la tarea
                          1,                    // Prioridad baja
                          nullptr,              // Manejador
                          1                     // Núcleo
  );

  if (_serialTaskHandle == nullptr) {
    Serial.println("[ERROR] Failed to create serial task.");
    return;
  }
  else {
    Serial.println("\nREADY");
  }

  robotManager.init();

  Serial.setTimeout(10000); // Configurar tiempo de espera para comandos seriales
}

void RobotArm::serialTask(void* pvParameters)
{
  RobotArm* self = static_cast<RobotArm*>(pvParameters);

  while (true) {
    if (Serial.available()) {
      char cmd[READ_BUFFER_SIZE] = {0};
      Serial.readBytesUntil('\n', cmd, sizeof(cmd));
      cmd[READ_BUFFER_SIZE - 1] = '\0'; // Asegurar terminación de cadena
      int len                   = strlen(cmd);
      while (len > 0 && (cmd[len - 1] == '\r' || cmd[len - 1] == '\n' || cmd[len - 1] == ' ')) {
        cmd[len - 1] = '\0';
        len--;
      }
      for (int i = 0; i < len; i++) {
        cmd[i] = toupper((unsigned char)cmd[i]);
      }
      if (xQueueSend(self->_commandQueue, &cmd, portMAX_DELAY) != pdPASS) {
        Serial.println("[ERROR] Failed to enqueue command.");
      }
      memset(cmd, 0, sizeof(cmd)); // Vaciar el buffer después de enviar a la cola
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void RobotArm::processCommandTask(void* pvParameters)
{
  RobotArm* self = static_cast<RobotArm*>(pvParameters);
  char      cmd[READ_BUFFER_SIZE];
  while (true) {
    if (xQueueReceive(self->_commandQueue, &cmd, portMAX_DELAY) == pdPASS) {
      char action[10] = {0}, param_one[50] = {0}, param_two[50] = {0}, param_three[50] = {0}, param_four[50] = {0}, param_five[50] = {0},
           param_six[50] = {0};
      sscanf(cmd, "%9[^:]:%49[^:]:%49[^:]:%49[^:]:%49[^:]:%49[^:]:%49[^:]", action, param_one, param_two, param_three, param_four, param_five,
             param_six);
      if (strcmp(action, SETUP) == 0 && strlen(param_one) > 0 && strlen(param_two) > 0 && strlen(param_three) == 0 && strlen(param_four) == 0 &&
          strlen(param_five) == 0 && strlen(param_six) == 0) {
        Serial.printf("[INFO] Command received: Action=%s, Parameter=%s, Value=%s\n", action, param_one, param_two);
        self->setupCommand(param_one, param_two);
      }
      else if (strcmp(action, PLACE) == 0 && strlen(param_one) > 0 && strlen(param_two) > 0 && strlen(param_three) > 0) {
        Serial.printf("[INFO] Command received: Action=%s, Q1=%s, Q2=%s, Q3=%s, Q4=%s, Q5=%s, Q6=%s\n", action, param_one, param_two, param_three,
                      param_four, param_five, param_six);
        self->placeCommand(param_one, param_two, param_three, param_four, param_five, param_six);
      }
      else if (strcmp(action, READ) == 0 && strlen(param_one) > 0 && strlen(param_two) == 0 && strlen(param_three) == 0) {
        Serial.printf("[INFO] Command received: Action=%s, Parameter=%s\n", action, param_one);
        self->readCommand(param_one);
      }
      else if (strcmp(cmd, "?") == 0) {
        Serial.println("[INFO] Available commands:");
        Serial.println(" - SETUP:<param>:<value> to configure the robot arm.");
        Serial.println(" - PLACE:<q1>:<q2>:<q3>:<q4>:<q5>:<q6> to move all servos to specified positions.");
        Serial.println(" - READ:<variable> to read information from the robot arm.");
        Serial.println(" - ? to show this help message.");
      }
      else {
        Serial.printf("[ERROR] Invalid command format: %s\n", cmd);
      }
    }
  }
}

void RobotArm::setupCommand(const char* param, const char* value)
{
  if (strcmp(param, SERVO_1) == 0) {
    robotManager.setServoTargetPosition(1, atoi(value));
    Serial.printf("ANGLE:SERVO1:%d\n", robotManager.getServoPosition(1));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO1:%d\n", robotManager.getServoPositionWithOffset(1));
  }
  else if (strcmp(param, SERVO_2) == 0) {
    robotManager.setServoTargetPosition(2, atoi(value));
    Serial.printf("ANGLE:SERVO2:%d\n", robotManager.getServoPosition(2));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO2:%d\n", robotManager.getServoPositionWithOffset(2) * (-1));
  }
  else if (strcmp(param, SERVO_3) == 0) {
    robotManager.setServoTargetPosition(3, atoi(value));
    Serial.printf("ANGLE:SERVO3:%d\n", robotManager.getServoPosition(3));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO3:%d\n", robotManager.getServoPositionWithOffset(3));
  }
  else if (strcmp(param, SERVO_4) == 0) {
    robotManager.setServoTargetPosition(4, atoi(value));
    Serial.printf("ANGLE:SERVO4:%d\n", robotManager.getServoPosition(4));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO4:%d\n", robotManager.getServoPositionWithOffset(4));
  }
  else if (strcmp(param, SERVO_5) == 0) {
    robotManager.setServoTargetPosition(5, atoi(value));
    Serial.printf("ANGLE:SERVO5:%d\n", robotManager.getServoPosition(5));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO5:%d\n", robotManager.getServoPositionWithOffset(5) * (-1));
  }
  else if (strcmp(param, SERVO_6) == 0) {
    robotManager.setServoTargetPosition(6, atoi(value));
    Serial.printf("ANGLE:SERVO6:%d\n", robotManager.getServoPosition(6));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO6:%d\n", robotManager.getServoPositionWithOffset(6));
  }
  else if (strcmp(param, OFFSET_1) == 0) {
    robotManager.setServoOffset(1, atoi(value));
    Serial.printf("OFFSET:SERVO1:%d\n", robotManager.getServoOffset(1));
  }
  else if (strcmp(param, OFFSET_2) == 0) {
    robotManager.setServoOffset(2, atoi(value));
    Serial.printf("OFFSET:SERVO2:%d\n", robotManager.getServoOffset(2));
  }
  else if (strcmp(param, OFFSET_3) == 0) {
    robotManager.setServoOffset(3, atoi(value));
    Serial.printf("OFFSET:SERVO3:%d\n", robotManager.getServoOffset(3));
  }
  else if (strcmp(param, OFFSET_4) == 0) {
    robotManager.setServoOffset(4, atoi(value));
    Serial.printf("OFFSET:SERVO4:%d\n", robotManager.getServoOffset(4));
  }
  else if (strcmp(param, OFFSET_5) == 0) {
    robotManager.setServoOffset(5, atoi(value));
    Serial.printf("OFFSET:SERVO5:%d\n", robotManager.getServoOffset(5));
  }
  else if (strcmp(param, OFFSET_6) == 0) {
    robotManager.setServoOffset(6, atoi(value));
    Serial.printf("OFFSET:SERVO6:%d\n", robotManager.getServoOffset(6));
  }
  else if (strcmp(param, SERVO_SPEED) == 0) {
    robotManager.setMovementSpeed(atoi(value));
    Serial.printf("SPEED:%d\n", robotManager.getMovementSpeed());
  }
  else if (strcmp(param, PLACE_1) == 0) {
    robotManager.setPlacePosition(1, atoi(value));
    Serial.printf("PLACE:SERVO1:%d\n", robotManager.getPlacePosition(1));
  }
  else if (strcmp(param, PLACE_2) == 0) {
    robotManager.setPlacePosition(2, atoi(value));
    Serial.printf("PLACE:SERVO2:%d\n", robotManager.getPlacePosition(2));
  }
  else if (strcmp(param, PLACE_3) == 0) {
    robotManager.setPlacePosition(3, atoi(value));
    Serial.printf("PLACE:SERVO3:%d\n", robotManager.getPlacePosition(3));
  }
  else if (strcmp(param, PLACE_4) == 0) {
    robotManager.setPlacePosition(4, atoi(value));
    Serial.printf("PLACE:SERVO4:%d\n", robotManager.getPlacePosition(4));
  }
  else if (strcmp(param, PLACE_5) == 0) {
    robotManager.setPlacePosition(5, atoi(value));
    Serial.printf("PLACE:SERVO5:%d\n", robotManager.getPlacePosition(5));
  }
  else if (strcmp(param, PLACE_6) == 0) {
    robotManager.setPlacePosition(6, atoi(value));
    Serial.printf("PLACE:SERVO6:%d\n", robotManager.getPlacePosition(6));
  }
  else if (strcmp(param, ABORT) == 0) {
    Serial.println("[INFO] Aborting current movements.");
    _abortRequested = true;
  }
  else {
    Serial.printf("[ERROR] Unknown parameter: %s\n", param);
  }
}

void RobotArm::placeCommand(const char* q1, const char* q2, const char* q3, const char* q4, const char* q5, const char* q6)
{
  Serial.printf("[PLACE] Moving to positions Q1:%s Q2:%s Q3:%s Q4:%s Q5:%s Q6:%s\n", q1, q2, q3, q4, q5, q6);

  _isRunning      = true;
  _abortRequested = false;
  robotManager.setServoTargetPosition(5, atoi(q5));
  robotManager.setServoTargetPosition(3, atoi(q3));
  robotManager.setServoTargetPosition(2, atoi(q2));
  robotManager.setServoTargetPosition(1, atoi(q1));

  vTaskDelay(4000 / portTICK_PERIOD_MS); // Small delay to ensure the arm is in the correct position before moving the last claw servo

  if (_abortRequested) {
    Serial.println("[PLACE] Movement aborted before moving Servo 6.");
    _isRunning = false;
    return;
  }
  robotManager.setServoTargetPosition(6, atoi(q6));

  // Wait for the claw servo to reach its position
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  // Check if an abort was requested during the movement
  if (_abortRequested) {
    Serial.println("[PLACE] Movement aborted.");
    _isRunning = false;
    return;
  }
  else
    Serial.println("[PLACE] Movement completed.");

  // Move the claw to a upper position (motor 2 to offset) before moving to place positions
  robotManager.setServoTargetPosition(1, robotManager.getPlacePosition(1));
  robotManager.setServoTargetPosition(2, robotManager.getServoOffset(2));
  vTaskDelay(3000 / portTICK_PERIOD_MS); // Wait for the base

  // Move the arm to the place saved positions
  robotManager.setServoTargetPosition(2, robotManager.getPlacePosition(2));
  robotManager.setServoTargetPosition(3, robotManager.getPlacePosition(3));
  robotManager.setServoTargetPosition(5, robotManager.getPlacePosition(5));

  vTaskDelay(3000 / portTICK_PERIOD_MS); // Wait for the arm to reach the place positions

  if (_abortRequested) {
    Serial.println("[PLACE] Movement to place positions aborted.");
    _isRunning = false;
    return;
  }
  else {
    Serial.println("[PLACE] Moved to place positions successfully.");
  }

  // Open claw after placing to 130 degrees
  robotManager.setServoTargetPosition(6, 130);
  vTaskDelay(3000 / portTICK_PERIOD_MS);

  // Return to offset positions after placing
  robotManager.setServoTargetPosition(1, robotManager.getServoOffset(1));
  robotManager.setServoTargetPosition(2, robotManager.getServoOffset(2));
  robotManager.setServoTargetPosition(3, robotManager.getServoOffset(3));
  robotManager.setServoTargetPosition(4, robotManager.getServoOffset(4));
  robotManager.setServoTargetPosition(5, robotManager.getServoOffset(5));
  robotManager.setServoTargetPosition(6, robotManager.getServoOffset(6));
  _isRunning = false;
  Serial.print("PLACE_DONE\n");
}

void RobotArm::readCommand(const char* variable)
{
  Serial.printf("[READ] Reading variable: %s\n", variable);
  if (strcmp(variable, SERVO_ANGLES) == 0) {
    Serial.printf("ANGLE:SERVO1:%d\n", robotManager.getServoPosition(1));
    Serial.printf("ANGLE:SERVO2:%d\n", robotManager.getServoPosition(2));
    Serial.printf("ANGLE:SERVO3:%d\n", robotManager.getServoPosition(3));
    Serial.printf("ANGLE:SERVO4:%d\n", robotManager.getServoPosition(4));
    Serial.printf("ANGLE:SERVO5:%d\n", robotManager.getServoPosition(5));
    Serial.printf("ANGLE:SERVO6:%d\n", robotManager.getServoPosition(6));
  }
  else if (strcmp(variable, SERVO_ANGLES_WITH_OFFSET) == 0) {
    Serial.printf("ANGLE_WITH_OFFSET:SERVO1:%d\n", robotManager.getServoPositionWithOffset(1));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO2:%d\n", robotManager.getServoPositionWithOffset(2));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO3:%d\n", robotManager.getServoPositionWithOffset(3));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO4:%d\n", robotManager.getServoPositionWithOffset(4));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO5:%d\n", robotManager.getServoPositionWithOffset(5));
    Serial.printf("ANGLE_WITH_OFFSET:SERVO6:%d\n", robotManager.getServoPositionWithOffset(6));
  }
  else if (strcmp(variable, SERVO_OFFSETS) == 0) {
    Serial.printf("OFFSET:SERVO1:%d\n", robotManager.getServoOffset(1));
    Serial.printf("OFFSET:SERVO2:%d\n", robotManager.getServoOffset(2));
    Serial.printf("OFFSET:SERVO3:%d\n", robotManager.getServoOffset(3));
    Serial.printf("OFFSET:SERVO4:%d\n", robotManager.getServoOffset(4));
    Serial.printf("OFFSET:SERVO5:%d\n", robotManager.getServoOffset(5));
    Serial.printf("OFFSET:SERVO6:%d\n", robotManager.getServoOffset(6));
  }
  else {
    Serial.printf("[ERROR] Unknown variable: %s\n", variable);
  }
}
