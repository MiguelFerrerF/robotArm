/*
 * ESP32 Servo Example Using Arduino ESP32 Servo Library
 * John K. Bennett
 * March, 2017
 *
 * This sketch uses the Arduino ESP32 Servo Library to sweep 4 servos in sequence.
 *
 * Different servos require different pulse widths to vary servo angle, but the range is
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 *
 * Circuit:
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS.
 *
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pins
 * 22, 19, 23, & 18).
 *
 * In this example, we assume four Tower Pro SG90 small servos.
 * The published min and max for this servo are 500 and 2400, respectively.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
 * Experimentally, 550 and 2350 are pretty close to 0 and 180.
 */

#include <ESP32Servo.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// create four servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
// Published values for SG90 servos; adjust if needed
int minUs = 550;
int maxUs = 2550;

int  servo1Pin = 14;
int  servo2Pin = 27;
int  servo3Pin = 26;
int  servo4Pin = 25;
int  servo5Pin = 33;
int  pos       = 0; // position in degrees
void setup()
{
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.begin(115200);
  servo1.setPeriodHertz(50); // Standard 50hz servo
  servo2.setPeriodHertz(50); // Standard 50hz servo
  servo3.setPeriodHertz(50); // Standard 50hz servo
  servo4.setPeriodHertz(50); // Standard 50hz servo
  servo5.setPeriodHertz(50); // Standard 50hz servo
  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);
  servo3.attach(servo3Pin, minUs, maxUs);
  servo4.attach(servo4Pin, minUs, maxUs);
  servo5.attach(servo5Pin, minUs, maxUs);

  // task to read servo positions
  xTaskCreate(
    [](void*) {
      for (;;) {
        Serial.printf("Servo1: %d, Servo2: %d, Servo3: %d, Servo4: %d, Servo5: %d\r\n",
                      servo1.read(), servo2.read(), servo3.read(), servo4.read(), servo5.read());
        vTaskDelay(2 / portTICK_PERIOD_MS);
      }
    },
    "readServo", 4096, NULL, 1, NULL);
}

void loop()
{
  servo1.write(180);
  delay(5000);
  servo1.write(0);
  delay(5000);

  // for (pos = 0; pos <= 180; pos += 1) { // sweep from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   servo2.write(pos);
  //   Serial.printf("Servo2: %d\r\n", servo2.read());
  //   delay(10); // waits 20ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
  //   servo2.write(pos);
  //   Serial.printf("Servo2: %d\r\n", servo2.read());
  //   delay(10);
  // }

  // for (pos = 0; pos <= 180; pos += 1) { // sweep from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   servo3.write(pos);
  //   delay(10); // waits 20ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
  //   servo3.write(pos);
  //   delay(10);
  // }

  // for (pos = 0; pos <= 180; pos += 1) { // sweep from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   servo4.write(pos);
  //   delay(10); // waits 20ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
  //   servo4.write(pos);
  //   delay(10);
  // }
  // for (pos = 0; pos <= 180; pos += 1) { // sweep from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   servo5.write(pos);
  //   delay(10); // waits 20ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
  //   servo5.write(pos);
  //   delay(10);
  // }
}
