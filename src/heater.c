#include <pthread.h>
#include "heater.h"

int heater_temperature = 0;

pthread_mutex_t heater_mutex; // Mutex for accessing heater_temperature

void initialize_heater() {
  // Initialize mutex
  pthread_mutex_init(&heater_mutex, NULL);
}

void cleanup_heater() {
  // Destroy mutex
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
