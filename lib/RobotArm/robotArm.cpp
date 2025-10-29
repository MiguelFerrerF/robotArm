
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
      char action[10] = {0}, param_one[50] = {0}, param_two[50] = {0}, param_three[50] = {0};
      sscanf(cmd, "%9[^:]:%49[^:]:%49[^:]:%49s", action, param_one, param_two, param_three);
      if (strcmp(action, SETUP) == 0 && strlen(param_one) > 0 && strlen(param_two) > 0 &&
          strlen(param_three) == 0) {
        Serial.printf("[INFO] Command received: Action=%s, Parameter=%s, Value=%s\n", action,
                      param_one, param_two);
        self->setupCommand(param_one, param_two);
      }
      else if (strcmp(action, MOVE) == 0 && strlen(param_one) > 0 && strlen(param_two) > 0 &&
               strlen(param_three) > 0) {
        Serial.printf("[INFO] Command received: Action=%s, X=%s, Y=%s, Z=%s\n", action, param_one,
                      param_two, param_three);
        self->moveCommand(param_one, param_two, param_three);
      }
      else if (strcmp(action, READ) == 0 && strlen(param_one) > 0 && strlen(param_two) == 0 &&
               strlen(param_three) == 0) {
        Serial.printf("[INFO] Command received: Action=%s, Parameter=%s\n", action, param_one);
        self->readCommand(param_one);
      }
      else if (strcmp(cmd, "?") == 0) {
        Serial.println("[INFO] Available commands:");
        Serial.println(" - SETUP:<param>:<value> to configure the robot arm.");
        Serial.println(" - SET:<x>:<y>:<z> to move the robot arm to position (x,y,z).");
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
  int offset1 = 30;
  int offset2 = 103;
  int offset3 = 90;
  int offset4 = 134;
  int offset5 = 69;
  int offset6 = 0;

  Serial.printf("[SETUP] Setting %s to %s\n", param, value);

  if (strcmp(param, "SERVO1") == 0) {
    int pos = atoi(value);
    robotManager.setServoPosition(1, pos);
    Serial.printf("ANGLE:SERVO1:%d\n", robotManager.getServoPosition(1)-offset1);
  }
  else if (strcmp(param, "SERVO2") == 0) {
    int pos = atoi(value);
    robotManager.setServoPosition(2, pos);
    Serial.printf("ANGLE:SERVO2:%d\n", robotManager.getServoPosition(2)-offset2);
  }
  else if (strcmp(param, "SERVO3") == 0) {
    int pos = atoi(value);
    robotManager.setServoPosition(3, pos);
    Serial.printf("ANGLE:SERVO3:%d\n", robotManager.getServoPosition(3)-offset3);
  }
  else if (strcmp(param, "SERVO4") == 0) {
    int pos = atoi(value);
    robotManager.setServoPosition(4, pos);
    Serial.printf("ANGLE:SERVO4:%d\n", robotManager.getServoPosition(4)-offset4);
  }
  else if (strcmp(param, "SERVO5") == 0) {
    int pos = atoi(value);
    robotManager.setServoPosition(5, pos);
    Serial.printf("ANGLE:SERVO5:%d\n", robotManager.getServoPosition(5)-offset5);
  }
  else if (strcmp(param, "SERVO6") == 0) {
    int pos = atoi(value);
    robotManager.setServoPosition(6, pos);
    Serial.printf("ANGLE:SERVO6:%d\n", robotManager.getServoPosition(6)-offset6);
  }
  else {
    Serial.printf("[ERROR] Unknown parameter: %s\n", param);
  }
}

void RobotArm::moveCommand(const char* x, const char* y, const char* z)
{
  Serial.printf("[MOVE] Moving to position X:%s Y:%s Z:%s\n", x, y, z);
}

void RobotArm::readCommand(const char* variable)
{
  Serial.printf("[READ] Reading variable: %s\n", variable);
}
