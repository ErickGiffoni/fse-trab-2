#include <stdio.h>
#include <string.h>

#include "../lib/update_temp.h"
#include "../lib/csv.h"
#include "../lib/bme280.h"
#include "../lib/socket.h"
#include "../lib/events.h"

float T, H;

void handle_temp_and_humidity() {
    char message[200];

    get_temperature_and_humidity(&T, &H);
    write_csv_on_file(T, H);

    sprintf(message, "%s;%.2f;%.2f", UPDATE_TEMP_EVENT, T, H);
    
    send_message_to_server(message, strlen(message));
}