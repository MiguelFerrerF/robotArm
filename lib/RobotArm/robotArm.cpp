
#include "robotArm.h"

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

  Serial.setTimeout(10000); // Configurar tiempo de espera para comandos seriales
}

void RobotArm::serialTask(void* pvParameters)
{
  RobotArm* self = static_cast<RobotArm*>(pvParameters);

  while (true) {
    if (Serial.available()) {
      char cmd[READ_BUFFER_SIZE] = {0}; // Inicializar el buffer para evitar residuos
      Serial.readBytesUntil('\n', cmd, sizeof(cmd));
      cmd[READ_BUFFER_SIZE - 1] = '\0'; // Asegurar terminación de cadena
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
      char action[10], param_one[50], param_two[50], param_three[50];
      sscanf(cmd, "%9[^:]:%49[^:]:%49[^:]:%49s", action, param_one, param_two, param_three);
      if (strcmp(action, SETUP) == 0 && strlen(param_one) > 0 && strlen(param_two) > 0 &&
          strlen(param_three) == 0) {
        Serial.printf("[INFO] Command received: Action=%s, Param1=%s, Param2=%s\n", action,
                      param_one, param_two);
        self->setupCommand(param_one, param_two);
      }
      else if (strcmp(action, SET) == 0 && strlen(param_one) > 0 && strlen(param_two) > 0 &&
               strlen(param_three) > 0) {
        Serial.printf("[INFO] Command received: Action=%s, Param1=%s, Param2=%s, Param3=%s\n",
                      action, param_one, param_two, param_three);
        self->moveCommand(param_one, param_two, param_three);
      }
      else if (strcmp(action, READ) == 0 && strlen(param_one) > 0 && strlen(param_two) == 0 &&
               strlen(param_three) == 0) {
        Serial.printf("[INFO] Command received: Action=%s, Param1=%s\n", action, param_one);
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
  Serial.printf("[SETUP] Setting %s to %s\n", param, value);
}

void RobotArm::moveCommand(const char* x, const char* y, const char* z)
{
  Serial.printf("[MOVE] Moving to position X:%s Y:%s Z:%s\n", x, y, z);
}

void RobotArm::readCommand(const char* variable)
{
  Serial.printf("[READ] Reading variable: %s\n", variable);
}
