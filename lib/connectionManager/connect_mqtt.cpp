#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_config.h"
#include <ArduinoJson.h>

#define MQTT_TIMEOUT 5000

WiFiClient espClient;
PubSubClient client(espClient);

PubSubClient get_pub_sub_client()
{
    return client;
}

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
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        if (client.connected())
        {
            Serial.println("[MQTT] - Already connected to the MQTT broker");
            continue;
        }
        client.setServer(MQTT_BROKER, MQTT_PORT);
        client.setCallback(callback);
        unsigned long startAttemptTime = millis();

        while (!client.connected() && millis() - startAttemptTime < MQTT_TIMEOUT)
        {
            if (client.connect(CLIENT_NAME, MQTT_USERNAME, MQTT_PASSWORD))
            {
                Serial.println("[MQTT] - Connect to the MQTT broker");
            }
            else
            {
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                Serial.println("[MQTT] - Connection failed");
                continue;
            }
        }

        client.subscribe(TOPIC_COMMAND);
    }
}

void send_data(void *parameter)
{
    while (true)
    {
        if (client.connected() == false)
        {
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            continue;
        }

        JsonDocument doc;
        String payload = "test data";

        // float watts = pzemConsumoCasa.power();
        // float amps = pzemConsumoCasa.current();
        // int volts = pzemConsumoCasa.voltage();

        // jsonDoc["corriente"] = amps;
        // jsonDoc["voltaje"] = volts;
        // jsonDoc["potencia"] = watts;

        serializeJson(doc, payload);
        client.publish(TOPIC_STATE, (char *)payload.c_str());

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}