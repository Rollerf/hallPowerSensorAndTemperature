#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_config.h"
#include <ArduinoJson.h>

#define MQTT_TIMEOUT 5000

WiFiClient espClient;
PubSubClient client(espClient);

// TODO: Pending create the task to connect to the MQTT broker
// TODO: Pending testing the connection to the MQTT broker
void callback(char *topicCommand, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topicCommand);
    Serial.print("Message:");
    String payload_n;

    for (int i = 0; i < length; i++)
    {
        payload_n += (char)payload[i];
    }

    // TODO: Pending testing this instead of the for loop
    //  String payload_n((char *)payload, length);

    Serial.println(payload_n);
    Serial.println("-----------------------");

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, payload_n);

    if (error)
        return;
}

void connect_mqtt(void *parameter)
{
    while (true)
    {
        if (client.connected())
        {
            Serial.println("[MQTT] - Already connected to the MQTT broker");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            continue;
        }
        client.setServer(MQTT_BROKER, MQTT_PORT);
        client.setCallback(callback);
        unsigned long startAttemptTime = millis();

        while (!client.connected() && millis() - startAttemptTime < MQTT_TIMEOUT)
        {
            if (!client.connected())
            {
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                Serial.println("[MQTT] - Connection failed");
                continue;
            }
        }
        Serial.println("[MQTT] - Connected to the MQTT broker");
    }
}