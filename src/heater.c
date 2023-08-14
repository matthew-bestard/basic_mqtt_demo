#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "MQTTClient.h"
#include "utility.h"
#include "heater.h"

int heater_temperature = 0;

pthread_mutex_t heater_mutex; // Mutex for accessing heater_temperature
pthread_t heater_thread_id; // Thread UD fir the heater thread

void* heater_thread(void* arg) {
  while (1) {
    pthread_mutex_lock(&heater_mutex); // Lock mutex before reading heater_temperature
    printf("Current Heater Temperature: %d\n", heater_temperature);
    pthread_mutex_unlock(&heater_mutex);
    usleep(1000000); // Sleep for 1 second
  }
}

void initialize_heater() {
    pthread_mutex_init(&heater_mutex, NULL);
    
    // Create the heatert hread
    int rc = pthread_create(&heater_thread_id, NULL, heater_thread, NULL);
    if (rc != 0) {
      fprintf(stderr, "Failed to create heater thread: %d\n", rc);
      exit(EXIT_FAILURE);
    }
}

void cleanup_heater() {
  // Cancel the heater thread
  pthread_cancel(heater_thread_id);

  // Wait for the heater thread to complete
  pthread_join(heater_thread_id, NULL);

  // Destroy the mutex
  pthread_mutex_destroy(&heater_mutex);
}

int get_temperature() {
  int temperature = 0;
  
  pthread_mutex_lock(&heater_mutex);
  temperature = heater_temperature;
  pthread_mutex_unlock(&heater_mutex);

  return temperature;
}

void set_temperature(int new_temperature) {
  pthread_mutex_lock(&heater_mutex);
  heater_temperature = new_temperature;
  pthread_mutex_unlock(&heater_mutex);
}
