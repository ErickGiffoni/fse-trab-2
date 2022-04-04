#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <string.h>    
#include <softPwm.h>  /* include header file for software PWM */
#include <stdlib.h>
#include <unistd.h>
#include "../lib/gpio.h"
#include "../lib/socket.h"
#include "../lib/events.h"
#include "../lib/json_handler.h"

static int sensor_reference[6] = {
	LAMPADA_COZINHA, // GPIO 17
	LAMPADA_SALA, // GPIO 18
	LAMPADA_QUARTO_01, // GPIO 27
	LAMPADA_QUARTO_02, // GPIO 22
	AR_CONDICIONADO_QUARTO_01, // GPIO 13
	AR_CONDICIONADO_QUARTO_02, // GPIO 19
};

// int sensor_hallway_output[5] 	= {0,0,0,0,0};
// int sensor_hallway_input[7] 	= {0,0,0,0,0,0,0};

// int sensor_first_floor_output[4] = {0,0,0,0};
// int sensor_first_floor_input[6] 	= {0,0,0,0,0,0};

// void update_sensors_pins(){
// 	for(int i=0; i<5; i++){
// 		sensor_hallway_output[i] = 
// 	}
// 	for(int i=0; i<5; i++){
		
// 	}
// 	for(int i=0; i<5; i++){
		
// 	}
// 	for(int i=0; i<5; i++){
		
// 	}
// }

void init_gpio_device(int device_type, int pin_mode) {
	pinMode(device_type, pin_mode == 0 ? OUTPUT : INPUT);	/* set GPIO as output */
	softPwmCreate(device_type, 1, 100);	/* set PWM channel along with range*/
}

void init_all_gpio_devices(int isHallway){
	if(isHallway){
		init_gpio_device(get_device_gpio("outputs", "Lâmpada da Sala T01"), 0);
		init_gpio_device(get_device_gpio("outputs", "Lâmpada da Sala T02"), 0);
		init_gpio_device(get_device_gpio("outputs", "Lâmpadas do Corredor Terreo"), 0);
		init_gpio_device(get_device_gpio("outputs", "Ar-Condicionado Terreo"), 0);
		init_gpio_device(get_device_gpio("outputs", "Aspersor de Água (Incêndio)"), 0);

		init_gpio_device(get_device_gpio("inputs", "Sensor de Presença"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Fumaça"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Janela T01"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Janela T02"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Porta Entrada"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Contagem de Pessoas Entrando no Prédio"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Contagem de Pessoas Saindo do Prédio"), 1);
	}
	else{
		init_gpio_device(get_device_gpio("outputs", "Lâmpada da Sala 101"), 0);
		init_gpio_device(get_device_gpio("outputs", "Lâmpada da Sala 102"), 0);
		init_gpio_device(get_device_gpio("outputs", "Lâmpadas do Corredor"), 0);
		init_gpio_device(get_device_gpio("outputs", "Ar-Condicionado (1º Andar)"), 0);

		init_gpio_device(get_device_gpio("inputs", "Sensor de Presença"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Fumaça"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Janela 101"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Janela 102"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Contagem de Pessoas Entrando no 2º Andar"), 1);
		init_gpio_device(get_device_gpio("inputs", "Sensor de Contagem de Pessoas Saindo do 2º Andar"), 1);
	}

	return;
}

void handle_gpio_device(int device_type, int intensity) {
    softPwmWrite (device_type, intensity);
	usleep(15000);
}

int get_device_state(int device_type) {
	return digitalRead(device_type);
}

void gpio_handle_message(char* payload) {
	char *temp_str;
	int sensor_reference_index, sensor_value;

    temp_str = strtok(payload, ";");
	sensor_reference_index = atoi(temp_str);
    
	temp_str = strtok(NULL, ";");
	sensor_value = atoi(temp_str);

	handle_gpio_device(sensor_reference[sensor_reference_index], sensor_value * 100);

	char message[100];
	int sensor_state;

	if (sensor_reference_index > 3) // Por algum motivo os ar-condicionados sempre retornam 0 :/
		sensor_state = sensor_value;
	else
		sensor_state = get_device_state(sensor_reference_index);
	
	sprintf(message, "%s;%d;%d", GPIO_DEVICE_STATE, sensor_reference_index, sensor_state);

	send_message_to_server(message, strlen(message));
}