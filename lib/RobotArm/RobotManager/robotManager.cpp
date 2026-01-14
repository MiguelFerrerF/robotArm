#include "robotManager.h"

long mapRange(long x, long in_min, long in_max, long out_min, long out_max)
{
  // Asegurarse de que x está dentro del rango
  if (x < in_min)
    return out_min;
  if (x > in_max)
    return out_max;
  // Fórmula de mapeo lineal
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

RobotManager::RobotManager()
{
  // Init objective positions
  for (int i = 0; i < 6; i++) {
    _targetPosition[i] = -1;
  }
}

void RobotManager::startServoTasks()
{
  // Crear y anclar las tareas de movimiento a un núcleo
  const char* task_names[] = {"MoveServo1", "MoveServo2", "MoveServo3", "MoveServo4", "MoveServo5", "MoveServo6"};

  for (int i = 0; i < 6; ++i) {
    _taskParams[i].servo_id     = i + 1; // ID del servo: 1 a 6
    _taskParams[i].robotManager = this;

    // Crear la tarea para el servo i+1. Prioridad baja para el movimiento.
    if (xTaskCreatePinnedToCore(moveServoTask, task_names[i], 2048, &_taskParams[i], tskIDLE_PRIORITY, &_servoTaskHandles[i], 0) != pdPASS) {
      Serial.printf("[ERROR] Failed to create task for Servo %d.\n", i + 1);
    }
  }
}

void RobotManager::init()
{
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);
  servo4.setPeriodHertz(50);
  servo5.setPeriodHertz(50);
  servo6.setPeriodHertz(50);
  servo1.attach(SERVO1_PIN, MG995_uS_LOW, MG995_uS_HIGH);
  servo2.attach(SERVO2_PIN, MG995_uS_LOW, MG995_uS_HIGH);
  servo3.attach(SERVO3_PIN, MG995_uS_LOW, MG995_uS_HIGH);
  servo4.attach(SERVO4_PIN, MG90S_uS_LOW, MG90S_uS_HIGH);
  servo5.attach(SERVO5_PIN, MG90S_uS_LOW, MG90S_uS_HIGH);
  servo6.attach(SERVO6_PIN, MG90S_uS_LOW, MG90S_uS_HIGH);

  // Read initial positions
  _targetPosition[0] = servo1.read();
  _targetPosition[1] = servo2.read();
  _targetPosition[2] = servo3.read();
  _targetPosition[3] = servo4.read();
  _targetPosition[4] = servo5.read();
  _targetPosition[5] = servo6.read();

  // Initialize Preferences
  if (preferences.begin("robot_arm", false)) {
    offset1        = preferences.getInt("offset1", SERVO_1_OFFSET);
    offset2        = preferences.getInt("offset2", SERVO_2_OFFSET);
    offset3        = preferences.getInt("offset3", SERVO_3_OFFSET);
    offset4        = preferences.getInt("offset4", SERVO_4_OFFSET);
    offset5        = preferences.getInt("offset5", SERVO_5_OFFSET);
    offset6        = preferences.getInt("offset6", SERVO_6_OFFSET);
    place1         = preferences.getInt("place1", 0);
    place2         = preferences.getInt("place2", 0);
    place3         = preferences.getInt("place3", 0);
    place4         = preferences.getInt("place4", 0);
    place5         = preferences.getInt("place5", 0);
    place6         = preferences.getInt("place6", 0);
    _movementSpeed = preferences.getInt("speed", DEFAULT_SPEED);
    preferences.end();
  }
  else {
    Serial.println("[ERROR] Failed to initialize Preferences.");
  }

  startServoTasks();
}

void RobotManager::moveServoTask(void* pvParameters)
{
  ServoTaskParams_t* params       = (ServoTaskParams_t*)pvParameters;
  RobotManager*      self         = params->robotManager;
  int                servo_id     = params->servo_id;
  Servo*             currentServo = nullptr;

  // Asignar el puntero al servo correcto
  switch (servo_id) {
    case 1:
      currentServo = &self->servo1;
      break;
    case 2:
      currentServo = &self->servo2;
      break;
    case 3:
      currentServo = &self->servo3;
      break;
    case 4:
      currentServo = &self->servo4;
      break;
    case 5:
      currentServo = &self->servo5;
      break;
    case 6:
      currentServo = &self->servo6;
      break;
    default:
      vTaskDelete(NULL);
      return;
  }

  int current_pos = currentServo->read(); // Posición actual del servo (0-180)

  while (true) {
    int target_pos = self->_targetPosition[servo_id - 1];

    if (current_pos != target_pos) {
      // Determinar la dirección y moverse 1 grado
      if (current_pos < target_pos) {
        current_pos++;
      }
      else {
        current_pos--;
      }

      // Aplicar la lógica de inversión para los servos 2 y 5 (180 - posición)
      int write_pos = current_pos;
      if (servo_id == 2 || servo_id == 5) {
        write_pos = 180 - current_pos;
      }

      // Escribir la nueva posición al servo
      currentServo->write(write_pos);
    }

    // Calcular el retardo basado en la velocidad (1=50ms, 100=1ms)
    int speed    = self->_movementSpeed;
    int delay_ms = mapRange(speed, 1, 100, MOVEMENT_DELAY_MAX, MOVEMENT_DELAY_MIN);

    // Si la posición es objetivo, se usa un retardo mayor para reducir el uso de CPU.
    if (current_pos == target_pos) {
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    else {
      // Retardo entre cada paso de 1 grado para controlar la velocidad
      vTaskDelay(delay_ms / portTICK_PERIOD_MS);
    }
  }
}

void RobotManager::setServoTargetPosition(int servo, int position)
{
  // Limitar la posición al rango 0-180
  if (position < SERVO_MIN_ANGLE)
    position = SERVO_MIN_ANGLE;
  if (position > SERVO_MAX_ANGLE)
    position = SERVO_MAX_ANGLE;

  if (servo >= 1 && servo <= 6) {
    // Solo establece la posición objetivo. La tarea moverá el servo suavemente.
    _targetPosition[servo - 1] = position;
  }
  else {
    Serial.println("[ERROR] Invalid servo number.");
  }
}

// **Implementación de las funciones de velocidad**
void RobotManager::setMovementSpeed(int speed)
{
  if (speed < 1)
    speed = 1;
  if (speed > 100)
    speed = 100;
  _movementSpeed = speed;

  // Guardar en Preferences para que persista
  if (preferences.begin("robot_arm", false)) {
    preferences.putInt("speed", speed);
    preferences.end();
  }
}

int RobotManager::getMovementSpeed()
{
  return _movementSpeed;
}

int RobotManager::getServoPosition(int servo)
{
  switch (servo) {
    case 1:
      return servo1.read();
    case 2:
      return 180 - servo2.read();
    case 3:
      return servo3.read();
    case 4:
      return servo4.read();
    case 5:
      return 180 - servo5.read();
    case 6:
      return servo6.read();
    default:
      Serial.println("[ERROR] Invalid servo number.");
      return -1;
  }
}

int RobotManager::getServoPositionWithOffset(int servo)
{
  switch (servo) {
    case 1:
      return servo1.read() - offset1;
    case 2:
      return servo2.read() - offset2;
    case 3:
      return servo3.read() - offset3;
    case 4:
      return servo4.read() - offset4;
    case 5:
      return servo5.read() - offset5;
    case 6:
      return servo6.read() - offset6;
    default:
      Serial.println("[ERROR] Invalid servo number.");
      return -1;
  }
}

void RobotManager::setServoOffset(int servo, int offset)
{
  switch (servo) {
    case 1:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("offset1", offset);
        preferences.end();
      }
      offset1 = offset;
      break;
    case 2:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("offset2", 180 - offset);
        preferences.end();
      }
      offset2 = 180 - offset;
      break;
    case 3:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("offset3", offset);
        preferences.end();
      }
      offset3 = offset;
      break;
    case 4:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("offset4", offset);
        preferences.end();
      }
      offset4 = offset;
      break;
    case 5:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("offset5", 180 - offset);
        preferences.end();
      }
      offset5 = 180 - offset;
      break;
    case 6:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("offset6", offset);
        preferences.end();
      }
      offset6 = offset;
      break;
    default:
      Serial.println("[ERROR] Invalid servo number.");
      break;
  }
}

int RobotManager::getServoOffset(int servo)
{
  switch (servo) {
    case 1:
      return offset1;
    case 2:
      return (180 - offset2);
    case 3:
      return offset3;
    case 4:
      return offset4;
    case 5:
      return (180 - offset5);
    case 6:
      return offset6;
    default:
      Serial.println("[ERROR] Invalid servo number.");
      return 0;
  }
}

void RobotManager::setPlacePosition(int servo, int angle)
{
  switch (servo) {
    case 1:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("place1", angle);
        preferences.end();
      }
      place1 = angle;
      break;
    case 2:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("place2", angle);
        preferences.end();
      }
      place2 = angle;
      break;
    case 3:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("place3", angle);
        preferences.end();
      }
      place3 = angle;
      break;
    case 4:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("place4", angle);
        preferences.end();
      }
      place4 = angle;
      break;
    case 5:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("place5", angle);
        preferences.end();
      }
      place5 = angle;
      break;
    case 6:
      if (preferences.begin("robot_arm", false)) {
        preferences.putInt("place6", angle);
        preferences.end();
      }
      place6 = angle;
      break;
    default:
      Serial.println("[ERROR] Invalid servo number.");
      break;
  }
}

int RobotManager::getPlacePosition(int servo)
{
  switch (servo) {
    case 1:
      return place1;
    case 2:
      return place2;
    case 3:
      return place3;
    case 4:
      return place4;
    case 5:
      return place5;
    case 6:
      return place6;
    default:
      Serial.println("[ERROR] Invalid servo number.");
      return -1;
  }
}