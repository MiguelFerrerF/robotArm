#include "robotManager.h"

RobotManager::RobotManager()
{
}

void RobotManager::init()
{
  servo1.setPeriodHertz(50); // Standard 50hz servo
  servo2.setPeriodHertz(50); // Standard 50hz servo
  servo3.setPeriodHertz(50); // Standard 50hz servo
  servo4.setPeriodHertz(50); // Standard 50hz servo
  servo5.setPeriodHertz(50); // Standard 50hz servo
  servo6.setPeriodHertz(50); // Standard 50hz servo
  servo1.attach(SERVO1_PIN, 550, 2550);
  servo2.attach(SERVO2_PIN, 550, 2550);
  servo3.attach(SERVO3_PIN, 550, 2550);
  servo4.attach(SERVO4_PIN, 550, 2550);
  servo5.attach(SERVO5_PIN, 550, 2550);
  servo6.attach(SERVO6_PIN, 550, 2550);
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
