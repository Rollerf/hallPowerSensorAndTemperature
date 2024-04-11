#include "wifi_config.h"
#include <WiFi.h>
#define WIFI_TIMEOUT 5000

void connect_wifi(void *parameter)
{
    while (true)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("[WIFI] - Already connected to the WiFi network");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            continue;
        }
        WiFi.mode(WIFI_STA);
        WiFi.begin(SSID, PASSWORD);
        unsigned long startAttemptTime = millis();

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT)
        {
            if (WiFi.status() != WL_CONNECTED)
            {
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                Serial.println("[WIFI] - Connection failed");
                continue;
            }
        }
        Serial.println("[WIFI] - Connected to the WiFi network");
    }
}