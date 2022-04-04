#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>

#include <sys/types.h>    // AF_INET, SOCK_STREAM
#include <sys/socket.h>   // socket(), connect()
#include <arpa/inet.h>    // htons(), inet_addr()
#include <unistd.h>       // close()

#include "../lib/socket.h"
#include "../lib/socket_handler.h"

#define MAX 200
#define PORT 10025
#define SA struct sockaddr
#define SERVER_IP "192.168.0.53"
#define MESSAGE_WAIT_TIME_USECONDS 150000


int sockfd;

int init_socket()
{
    struct sockaddr_in servaddr;
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Falha ao criar socket...\n");
        exit(0);
    }
    else
        printf("Socket criado com sucesso!\n");
    bzero(&servaddr, sizeof(servaddr));
  
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(PORT);
  
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Conexão com o servidor central falhou\n");
        exit(0);
    }
    else
        printf("Conectado ao servidor central\n");

    return 0;
}

void* handle_socket_received_messages()
{
    int dc_times = 0;

    char buff[MAX];

    while(1) {
        bzero(buff, sizeof(buff));

        if (recv(sockfd, buff, sizeof(buff), 0) < 0){
            dc_times++;
        }
        else if (buff[0] == '\0') {
            dc_times++;
        }
        else {
            char date_time[35];
            time_t t = time(NULL);
            
            struct tm dtm = *localtime(&t);
            sprintf(date_time, "%d-%02d-%02d %02d:%02d:%02d", 
                dtm.tm_year + 1900, dtm.tm_mon + 1, dtm.tm_mday, dtm.tm_hour, dtm.tm_min, dtm.tm_sec);
            
            printf("[%s] Msg from server: %s\n", date_time, buff);
            handle_socket_message(buff);
        }

        usleep(MESSAGE_WAIT_TIME_USECONDS);
    }

    return 0;
}

void send_message_to_server(char* message, int messageSize) {
    if (sockfd == -1) {
       printf("Erro ao enviar msg. Socket não conectado...\n"); 
    }
    else {
        int result = send(sockfd, message, messageSize, 0);

        if (!result) {
            printf("Erro ao enviar msg\n");
        }
            
    }
};

void close_socket() {
    close(sockfd);
}
