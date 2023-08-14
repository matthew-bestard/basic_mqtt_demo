cmake_minimum_required(VERSION 3.10)
project(basic_mqtt_demo)

set(CMAKE_C_STANDARD 11)

include_directories(include)
link_libraries(paho-mqtt3c)

add_executable(basic_mqtt_demo src/main.c src/utility.c)
