#include <PubSubClient.h>

void connect_mqtt(void *parameter);
void send_data(void *parameter);

extern PubSubClient client;