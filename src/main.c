#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <MQTTClient.h>
#include "utility.h"
#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "mb-test-client-1"
#define TOPIC       "test/mb-test"
#define QOS         1
#define TIMEOUT     10000L

volatile int finished = 0;

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
  printf("--------------------------------------------------------------------------------\n");
  printf("Message arrived on topic: %s\n", topicName);
  printf("Message content: %.*s", message->payloadlen, (char*)message->payload);
  printf("--------------------------------------------------------------------------------\n");
  
  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

void sigint_handler(int sig) {
    finished = 1;
}

int main(int argc, char* argv[])
{
  signal(SIGINT, sigint_handler);
  
  MQTTClient client;
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
  int rc;
  int ch;
  
  rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
			 MQTTCLIENT_PERSISTENCE_NONE, NULL);
  handle_mqtt_error(rc, "Unable to create MQTT client.");
  
  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;
  
  rc = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
  handle_mqtt_error(rc, "Failed to set callbacks.");
  
  rc = MQTTClient_connect(client, &conn_opts);
  handle_mqtt_error(rc, "Failed to connect.");
  
  printf("Subscribed on topic: %s\n", TOPIC);
  printf("Using QOS of: %d\n", QOS);
  rc = MQTTClient_subscribe(client, TOPIC, QOS);
  handle_mqtt_error(rc, "Failed to Subscribe.");
  
  while (!finished) {
    
  }
  
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
  
  return rc;
}
