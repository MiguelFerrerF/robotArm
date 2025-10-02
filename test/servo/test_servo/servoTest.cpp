#include "unity.h"
#include <Arduino.h>
#include <ESP32Servo.h>

Servo testServo;

#define SERVO_PIN 14

void testServoAttach()
{
  ESP32PWM::allocateTimer(0);
  testServo.setPeriodHertz(50);           // standard 50 hz servo
  testServo.attach(SERVO_PIN, 500, 2500); // attaches the servo on pin 14 to the servo object
  String msg = "Servo is not attached to pin " + String(SERVO_PIN);
  TEST_ASSERT_TRUE_MESSAGE(testServo.attached(), msg.c_str());
}

void testServoRead()
{
  int angle = testServo.read();
  Serial.printf("Angle: %d\n", angle);
  TEST_ASSERT_TRUE_MESSAGE((angle >= 0 && angle <= 180),
                           "Servo angle read is not between 0 and 180 degrees");
}

void testServoWrite()
{
  testServo.write(45);
  int angle = testServo.read();
  TEST_ASSERT_EQUAL_INT_MESSAGE(45, angle, "Servo angle read is not equal to 45");
}

void setup()
{
  Serial.begin(115200);
  UNITY_BEGIN();

  RUN_TEST(testServoAttach);
  RUN_TEST(testServoRead);
  RUN_TEST(testServoWrite);

  UNITY_END();
  delay(1000); // Delay to avoid spamming
}

void loop()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void setUp()
{
}

void tearDown()
{
}
