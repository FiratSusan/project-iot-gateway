#include "mqtt.h"


IG_Mqtt * IG_Mqtt_create() {
   IG_Mqtt * stack = (IG_Mqtt *) malloc(sizeof(IG_Mqtt));
    return stack;
}



void pubmsg(IG_Mqtt * stack, char * payload, int len, char * topic, int timeout) {

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    pubmsg.payload = payload;
    pubmsg.payloadlen = len;
    pubmsg.qos = stack->qos_level;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(stack->client, topic, &pubmsg, &token);
    int rc = MQTTClient_waitForCompletion(stack->client, token, timeout);
    printf("Message with delivery token %d delivered\n", token);

}

void disconnect(MQTTClient * client) {
MQTTClient_disconnect(*client, 10000);
MQTTClient_destroy(client);
}

