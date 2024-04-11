#include <Arduino.h>
#include <WiFi.h>
#include "connect_wifi.h"

#define WIFI_TIMEOUT 5000

void setup()
{
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
      connect_wifi,                 // Function that should be called
      "Connect Wifi",               // Name of the task (for debugging)
      5000,                         // Stack size (bytes)
      NULL,                         // Parameter to pass
      1,                            // Task priority
      NULL,                         // Task handle
      CONFIG_ARDUINO_RUNNING_CORE); // Run task on Arduino core
}

void loop()
{
  // Empty. Things are done in Tasks.
}