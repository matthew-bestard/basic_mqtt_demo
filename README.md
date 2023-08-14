# MQTT Heater Control System

This is a simple MQTT-based heater control system that subscribes to temperature control messages and displays the current heater temperature. The program uses the Eclipse Paho MQTT C library to connect to an MQTT broker and receive messages.

This is a simple demonstration so the MQTT connection and subscription parameters are hard coded for the sake of time.

This program will connect to the "broker.emqx.io" on port "1883" using the client-id "mb-test-client-1" and subscribe to the topic "mb-test/control/temperature" using a QOS of 1.

## Features

- Subscribes to an MQTT topic for temperature control messages.
- Displays the current heater temperature using a separate thread.
- Allows controlling the heater temperature by sending MQTT messages to the control topic.

## Requirements

- Eclipse Paho MQTT C library
- CMake (for building)

## Usage

Go into the build directory and run the following.

```c
./basic_mqtt_demo
```
