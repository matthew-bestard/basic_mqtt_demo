#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <MQTTClient.h>
#include <unistd.h>
#include "utility.h"
#include <heater.h> // Include heater header

#define ADDRESS       "tcp://broker.emqx.io:1883"
#define CLIENTID      "mb-test-client-1"
#define TOPIC         "test/mb-test"
#define CONTROL_TOPIC "mb-test/control/temperature" // New control topic
#define QOS           1
#define TIMEOUT       10000L

volatile int finished = 0;

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
  printf("--------------------------------------------------------------------------------\n");
  printf("Message arrived on topic: %s\n", topicName);
  printf("Message content: %.*s", message->payloadlen, (char*)message->payload);
  printf("--------------------------------------------------------------------------------\n");

  if(strcmp(topicName, CONTROL_TOPIC) == 0) {
    // Parse the incoming message paylaod as a integer (representng temperature)
    int parsed_temperature = atoi((char*)message->payload);

    set_temperature(parsed_temperature);

    printf("Heater temperature set to: %d\n", get_temperature());
  }

  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);
  return 1;
}

void sigint_handler(int sig) {
    finished = 1;
}

void print_status() {
  printf("Current Heater Temperature: %d\n", get_temperature());
}

int main(int argc, char* argv[])
{
  signal(SIGINT, sigint_handler);

  int rc;

  MQTTClient client;
  MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

  rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
			 MQTTCLIENT_PERSISTENCE_NONE, NULL);
  handle_mqtt_error(rc, "Unable to create MQTT client.");

  conn_opts.keepAliveInterval = 20;
  conn_opts.cleansession = 1;

  rc = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
  handle_mqtt_error(rc, "Failed to set callbacks.");

  rc = MQTTClient_connect(client, &conn_opts);
  handle_mqtt_error(rc, "Failed to connect.");

  printf("Subscribed on topic: %s, using QOS: %d\n", TOPIC, QOS);
  rc = MQTTClient_subscribe(client, TOPIC, QOS);
  handle_mqtt_error(rc, "Failed to Subscribe.");

  printf("Subscribed on control topic: %s\n", CONTROL_TOPIC);
  rc = MQTTClient_subscribe(client, CONTROL_TOPIC, QOS);
  handle_mqtt_error(rc, "Failed to Subscribe to Control Topic.");

  initialize_heater();

  while (!finished) {
    print_status();
    usleep(1000000); // Sleep for 1 second
  }

  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);

  // Clean up the heater module
  cleanup_heater();

  return rc;
}
