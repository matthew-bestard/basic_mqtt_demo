#ifndef HEATER_H
#define HEATER_H

// Declare the functions
void initialize_heater();
void cleanup_heater();
int get_temperature();
void set_temperature(int new_temperature);

#endif // HEATER_H
