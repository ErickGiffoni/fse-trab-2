#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>       // close()
#include "../lib/socket.h"
#include "../lib/socket_handler.h"
#include "../lib/events.h"
#include "../lib/sensors.h"
#include "../lib/menu.h"

#define PORT 10025
#define SERVER_IP "192.168.0.53"
#define MESSAGE_WAIT_TIME_USECONDS 150000
#define MAX_ACCEPTED_NO_MESSAGES 20 // ou 2 segundos sem receber nenhuma mensagem

struct sockaddr_in server_addr, client_addr;

char client_message[2000];

int socket_desc, client_sock;
 
volatile int connected = 0;

socklen_t client_size;

int init_socket(void)
{    
    memset(client_message, 0x0, sizeof(client_message));
    
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Error ao criar socket\n");
        return -1;
    }

    sprintf(terminal_messages, "Socket criado com sucesso");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Não foi possível fazer o bind com a porta especificada\n");
        exit(1);
    }

    sprintf(terminal_messages, "Binding finalizado");

    printf("[x] %s\n", terminal_messages);

    return 0;
}

void* wait_client() {
    connected = 0;

    // Apenas inicia os dados dos sensores em modo de espera 
    receive_all_sensors_data("22222222");
    
    if(listen(socket_desc, 1) < 0){
        printf("Erro ao escutar socket\n");
        exit(1);
    }

    sprintf(terminal_messages, "Aguardando conexão do servidor distribuido...");
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_sock < 0){
        printf("Can't accept\n");
        exit(1);
    }

    handle_messages();

    return 0;
}

void handle_messages() {
    connected = 1;

    int dc_times = 0;

    sprintf(terminal_messages, "Servidor distribuido connectado::  IP: %s e porta: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    while(1) {
        bzero(client_message, 2000);

        if (dc_times > MAX_ACCEPTED_NO_MESSAGES)
            break;
        
        // Receive client's message:
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
            dc_times++;
        }
        else if (client_message[0] == '\0') {
            dc_times++;
        }
        else {
            handle_socket_message(client_message);
        }

        usleep(MESSAGE_WAIT_TIME_USECONDS);
    }

    sprintf(terminal_messages, "Servidor distribuído desconectado. Aguardando reconexão...");
    wait_client();
}

void send_message(char* message, int messageSize) {
    if (client_sock != -1) {
       send(client_sock, message, messageSize, 0);
    }
};

void send_message_to_server(char* message, int messageSize) {
    if (client_sock == -1) {
       sprintf(terminal_messages, "Erro ao tentar alterar sensor. Servidor distribuído não conectado...\n"); 
    }
    else {
        send(client_sock, message, messageSize, 0);
    }
};

void close_socket() {
    send_message_to_server(SERVER_CLOSED, strlen(SERVER_CLOSED));

    sleep(1);

    close(client_sock);
    close(socket_desc);
}