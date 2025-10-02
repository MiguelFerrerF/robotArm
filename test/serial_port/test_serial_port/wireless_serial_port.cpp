#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <WiFi.h>
#include <unity.h>

const char* ssid     = "HUAWEI P20 lite";
const char* password = "miguelferrer";

AsyncWebServer server(80);
void           recvMsg(uint8_t* data, size_t len)
{
  String message = "";
  for (size_t i = 0; i < len; i++) {
    message += char(data[i]);
  }
  WebSerial.println("Received: " + message);
  if (message == "ON")
    digitalWrite(BUILTIN_LED, HIGH);
  if (message == "OFF")
    digitalWrite(BUILTIN_LED, LOW);
}

void setup()
{
  Serial.begin(115200);
  UNITY_BEGIN();

  pinMode(BUILTIN_LED, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(1000);
  WebSerial.begin(&server);
  WebSerial.onMessage(recvMsg);
  server.begin();
  WebSerial.println("WebSerial started");
  WebSerial.println("IP address: " + WiFi.localIP().toString());

  UNITY_END();
  delay(1000); // Delay to avoid spamming
}

void loop()
{
  WebSerial.println("Hello from ESP32!");
  vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void setUp()
{
}

void tearDown()
{
}
