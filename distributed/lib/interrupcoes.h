
#ifndef INTERRUPCOES_H_
#define INTERRUPCOES_H_

#define  SENSOR_PRESENCA_SALA 6 // GPIO 25
#define  SENSOR_PRESENCA_COZINHA 25 // GPIO 26
#define  SENSOR_ABERTURA_PORTA_COZINHA 21 // GPIO 5
#define  SENSOR_ABERTURA_JANELA_COZINHA 22 // GPIO 6
#define  SENSOR_ABERTURA_PORTA_SALA 26 // GPIO 12
#define  SENSOR_ABERTURA_JANELA_SALA 27 // GPIO 16
#define  SENSOR_ABERTURA_JANELA_QUARTO_01 28 // GPIO 20
#define  SENSOR_ABERTURA_JANELA_QUARTO_02 29 // GPIO 21

#define IGNORE_CHANGE_BELOW_USEC 10000

extern volatile int sensorData[];

void* init_interruptions_handler();

void update_sensor_data();

void send_sensor_data_to_server();

#endif /* INTERRUPCOES_H_ */