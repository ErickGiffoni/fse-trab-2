#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sensors.h"
#include "../lib/menu.h"

volatile int sensors_values[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
volatile float temperature = 0, humidity = 0;

char sensor_state[2][20] = { "desativado", "ativado" };

char output_sensor_names[10][50] = { 
	"Acionar Lâmpada da Sala T01",
	"Acionar Lâmpada da Sala T02",
	"Acionar Lâmpadas do Corredor Terreo",
	"Acionar Ar-Condicionado Terreo",
	"Acionar alarme",
	"Acionar alarme de incêndio",
	"Acionar Lâmpada da Sala 101",
	"Acionar Lâmpada da Sala 102",
	"Acionar Lâmpadas do Corredor 1o andar",
	"Acionar Ar-Condicionado (1º Andar)"
};

char input_sensor_names[13][55] = {
	"Sensor de Presença",
	"Sensor de Fumaça",
	"Sensor de Janela T01",
	"Sensor de Janela T02",
	"Sensor de Porta Entrada",
	"Sensor de Contagem de Pessoas Entrando no Prédio",
	"Sensor de Contagem de Pessoas Saindo do Prédio",
	"Sensor de Presença",
	"Sensor de Fumaça",
	"Sensor de Janela 101",
	"Sensor de Janela 102",
	"Sensor de Contagem de Pessoas Entrando no 2º Andar",
	"Sensor de Contagem de Pessoas Saindo do 2º Andar",
};

void receive_all_sensors_data(char * payload) {
    for (int i = 0; i < 10; i++) {
        sensors_values[i] =  (int) payload[i] - 48;
    }
}

void update_sensor_data(char * payload) {
   char *temp_str;
	int sensor_reference_index, sensor_value;

   temp_str = strtok(payload, ";");
	sensor_reference_index = atoi(temp_str);
    
	temp_str = strtok(NULL, ";");
	sensor_value = atoi(temp_str);

	sensors_values[sensor_reference_index] = sensor_value;
}

void update_temp_and_humidity(char * payload) {
   char *temp_str;

   temp_str = strtok(payload, ";");
	temperature = atof(temp_str);
    
	temp_str = strtok(NULL, ";");
	humidity = atof(temp_str);
}

void update_gpio_device_value(char * payload) {
   char *temp_str;
	int device_index, sensor_value;

   temp_str = strtok(payload, ";");
	device_index = atoi(temp_str);
    
	temp_str = strtok(NULL, ";");
	sensor_value = atoi(temp_str);

	sensors[device_index] = sensor_value;

	sprintf(terminal_messages, "Sensor %s %s.", output_sensor_names[device_index], sensor_state[sensor_value]);
}