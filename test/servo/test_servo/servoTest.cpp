#include "unity.h"
#include <Arduino.h>
#include <Servo.h>

Servo testServo;
void  testServoAttach()
{
  testServo.attach(14);
  TEST_ASSERT_TRUE_MESSAGE(testServo.attached(), "Servo is not attached to pin 14");
}

void testServoRead()
{
  int angle = testServo.read();
  TEST_ASSERT_TRUE_MESSAGE((angle >= 0 && angle <= 189),
                           "Servo angle read is not between 0 and 189 degrees");
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
