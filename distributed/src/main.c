#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <wiringPi.h> /* include wiringPi library */
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "../lib/bme280.h"
#include "../lib/socket.h"
#include "../lib/gpio.h"
#include "../lib/csv.h"
#include "../lib/interrupcoes.h"
#include "../lib/update_temp.h"

#include "../lib/cJSON.h"
#include "../lib/json_handler.h"

void handle_init(char *configFile);
void handle_close();
pthread_t thread_interruptions, thread_sockets;
pthread_t hallway_floor, first_floor;

double central_server_port;
double distributed_server_port;
char central_server_ip[13];
char distributed_server_ip[13];

int main(int argc, const char * argv[]) {

    /* initialize wiringPi setup */
    wiringPiSetup();
    printf("[.] main: wiringPi initiated\n");
    
    // pthread_create(&hallway_floor, NULL, &handle_init, "../config/configuracao_andar_terreo.json");
    // pthread_detach(hallway_floor);
    
    // pthread_create(&first_floor, NULL, &handle_init, "../config/configuracao_andar_1.json");
    // pthread_detach(first_floor);

    // handle_init();

    signal(SIGINT, handle_close);
    signal(SIGPIPE, SIG_IGN);

    pthread_create(&hallway_floor, NULL, &handle_init, "config/configuracao_andar_terreo.json");
    pthread_detach(hallway_floor);
    
    pthread_create(&first_floor, NULL, &handle_init, "config/configuracao_andar_1.json");
    pthread_detach(first_floor);

    pthread_create(&thread_interruptions, NULL, &init_interruptions_handler, NULL);
    pthread_detach(thread_interruptions);
    
    pthread_create(&thread_sockets, NULL, &handle_socket_received_messages, NULL);
    pthread_detach(thread_sockets);
    
    while(1) {
        sleep(1);
    }

    return 0;
}

void handle_init(char *configFile) {
    printf("\nIniciando execução, aguarde...\n");

    // /* initialize wiringPi setup */
    // wiringPiSetup();

    //edit
    copy_JSON_to_buffer(configFile);
    setup_server_from_JSON(
        central_server_ip,
        distributed_server_ip,
        &central_server_port,
        &distributed_server_port
    );
    //end edit

    init_socket();		

    /* initialize bme280 setup */
    // if (bme280_init(0x76))
	// {
    //     printf("\nErro ao iniciar BME. Encerrando...\n");
	// 	exit(1); // problem - quit
	// }
    // else 
    //     printf("BME280 aberto com sucesso.\n");

    /* initialize output devices */

    //edit
    int amIHallway = 0;
    char *hall = strstr(configFile, "terreo");
    if(hall != NULL){
        amIHallway = 1;
    }
    init_all_gpio_devices(amIHallway);

    // init_gpio_device(LAMPADA_COZINHA);
    // init_gpio_device(LAMPADA_SALA);
    // init_gpio_device(LAMPADA_QUARTO_01);
    // init_gpio_device(LAMPADA_QUARTO_02);
    // init_gpio_device(AR_CONDICIONADO_QUARTO_01);
    // init_gpio_device(AR_CONDICIONADO_QUARTO_02);
    //end edit

    /* handle csv files */
    handle_file_creation();

    sleep(1);
    
    /* send sensors data to sserver */
    send_sensor_data_to_server();
}

void handle_close() {
    printf("\nEncerrando execução...\n");
    close_bme();
    pthread_cancel(thread_interruptions);
    pthread_cancel(thread_sockets);

    pthread_cancel(hallway_floor);
    pthread_cancel(first_floor);

    close_socket();
    exit(0);
}
