#include <stdio.h>
#include <stdlib.h>
#include <MQTTClient.h>
#include "utility.h"

void handle_mqtt_error(int rc, const char *msg) {
  if (rc != MQTTCLIENT_SUCCESS) {
    fprintf(stderr, "Error: %s (Error code: %d)\n", msg, rc);
    exit(EXIT_FAILURE);
  }
}
