#include <Arduino.h>
#include <Servo.h>

#define LED_BUILTIN 2
#define SERVO_1_PIN 14
#define SERVO_2_PIN 27
#define SERVO_3_PIN 26
#define SERVO_4_PIN 25
#define SERVO_5_PIN 33
#define SERVO_6_PIN 32

Servo servo1; // create Servo object to control a servo
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

int pos = 0; // variable to store the servo position

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  servo1.attach(
      SERVO_1_PIN); // attaches the servo on pin 14 to the servo object
  servo2.attach(SERVO_2_PIN);
  servo3.attach(SERVO_3_PIN);
  servo4.attach(SERVO_4_PIN);
  servo5.attach(SERVO_5_PIN);
  servo6.attach(SERVO_6_PIN);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);         // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo1.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);         // waits 15 ms for the servo to reach the position
  }
}
