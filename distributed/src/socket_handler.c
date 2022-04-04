#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "../lib/socket_handler.h"
#include "../lib/events.h"
#include "../lib/gpio.h"
#include "../lib/update_temp.h"


void handle_socket_message(char* message) {
    char *event, payload[200], temp_message[200];
    strncpy(temp_message, message , strlen(message));

    event = strtok(message, ";");
    
    strncpy(payload, temp_message + (strlen(event) + 1), strlen(temp_message) - strlen(event));


    if (strcmp(event, SERVER_CLOSED) == 0) {
        kill(getpid(), SIGINT);
    }
    else if (strcmp(event, CHANGE_SENSOR_VALUE) == 0) {
        gpio_handle_message(payload);
    }
    else if (strcmp(event, GET_TEMP_EVENT) == 0) {
        handle_temp_and_humidity();
    }
}
