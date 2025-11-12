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

  // Initialize Preferences
  if (preferences.begin("robot_arm", false)) {
    offset1 = preferences.getInt("offset1", SERVO_1_OFFSET);
    offset2 = preferences.getInt("offset2", SERVO_2_OFFSET);
    offset3 = preferences.getInt("offset3", SERVO_3_OFFSET);
    offset4 = preferences.getInt("offset4", SERVO_4_OFFSET);
    offset5 = preferences.getInt("offset5", SERVO_5_OFFSET);
    offset6 = preferences.getInt("offset6", SERVO_6_OFFSET);
    preferences.end();
  }
  else {
    Serial.println("[ERROR] Failed to initialize Preferences.");
  }

  Serial.printf("Servo Offsets loaded: %d, %d, %d, %d, %d, %d\n", offset1, offset2, offset3, offset4, offset5, offset6);
}

void RobotManager::setServoPosition(int servo, int position)
{
  switch (servo) {
    case 1:
      servo1.write(position);
      break;
    case 2:
      servo2.write(180 - position);
      break;
    case 3:
      servo3.write(position);
      break;
    case 4:
      servo4.write(position);
      break;
    case 5:
      servo5.write(180 - position);
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