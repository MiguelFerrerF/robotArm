#include "robotManager.h"

RobotManager::RobotManager()
{
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
}

void RobotManager::setServoPosition(int servo, int position)
{
  switch (servo) {
    case 1:
      servo1.write(position);
      break;
    case 2:
      servo2.write(position);
      break;
    case 3:
      servo3.write(position);
      break;
    case 4:
      servo4.write(position);
      break;
    case 5:
      servo5.write(position);
      break;
    case 6:
      servo6.write(position);
      break;
    default:
      Serial.println("[ERROR] Invalid servo number.");
      break;
  }
}

int RobotManager::getServoPosition(int servo)
{
  switch (servo) {
    case 1:
      return servo1.read();
    case 2:
      return servo2.read();
    case 3:
      return servo3.read();
    case 4:
      return servo4.read();
    case 5:
      return servo5.read();
    case 6:
      return servo6.read();
    default:
      Serial.println("[ERROR] Invalid servo number.");
      return -1;
  }
}

void RobotManager::setServoOffset(int servo, int offset)
{
  switch (servo) {
    case 1:
      offset1 = offset;
      break;
    case 2:
      offset2 = offset;
      break;
    case 3:
      offset3 = offset;
      break;
    case 4:
      offset4 = offset;
      break;
    case 5:
      offset5 = offset;
      break;
    case 6:
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
      return offset2;
    case 3:
      return offset3;
    case 4:
      return offset4;
    case 5:
      return offset5;
    case 6:
      return offset6;
    default:
      Serial.println("[ERROR] Invalid servo number.");
      return 0;
  }
}

int RobotManager::getServoPositionWithOffset(int servo)
{
  return getServoPosition(servo) - getServoOffset(servo);
}
