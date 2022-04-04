#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "../lib/socket_handler.h"
#include "../lib/events.h"
#include "../lib/sensors.h"
#include "../lib/menu.h"

void handle_socket_message(char* message) {
    char *event, payload[200], temp_message[200];
    bzero(payload, 200);
    bzero(temp_message, 200);

    strncpy(temp_message, message , strlen(message));

    event = strtok(message, ";");
    
    strncpy(payload, temp_message + (strlen(event) + 1), strlen(temp_message) - strlen(event));

    if (strcmp(event, ALL_SENSORS_STATES) == 0) {
        receive_all_sensors_data(payload);
    }
    if (strcmp(event, SENSOR_CHANGE) == 0) {
        update_sensor_data(payload);
    }
    if (strcmp(event, UPDATE_TEMP_EVENT) == 0) {
        update_temp_and_humidity(payload);
    }
    if (strcmp(event, GPIO_DEVICE_STATE) == 0) {
        update_gpio_device_value(payload);
    }
}
