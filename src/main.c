#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <MQTTClient.h>
#include "utility.h"
#include "heater.h"

// later setup getopts to pass these with flags from terminal
#define ADDRESS       "tcp://broker.emqx.io:1883"
#define CLIENTID      "mb-test-client-1"
#define TOPIC         "mb-test/control/temperature"
#define QOS           1
#define TIMEOUT       10000L

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
  printf("Control message arrived, setting temperature to: %.*s", message->payloadlen, (char*)message->payload);

  // Unsafe, in the future will use strtol and validate integer
  int parsed_temperature = atoi((char*)message->payload);

  set_temperature(parsed_temperature);
  printf("Heater succesfully temperature set to: %d\n", get_temperature());

  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);

  return 1;
}

int main(int argc, char* argv[])
{
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

  initialize_heater();

  // Wait for Ctrl+C (SIGINT) to exit
  printf("Press Ctrl+C to exit...\n");
  int sig;
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigwait(&set, &sig);

  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);

  // Clean up the heater module
  cleanup_heater();

  return rc;
}
