#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <curses.h>
#include "../lib/events.h"
#include "../lib/socket.h"
#include "../lib/menu.h"
#include "../lib/csv.h"

void handle_init();
void handle_close();
pthread_t thread, thread_menu;

int main(int argc, const char * argv[]) {
    handle_init();

    signal(SIGINT, handle_close);
    signal(SIGPIPE, SIG_IGN);


    pthread_create(&thread, NULL, &wait_client, NULL);
    pthread_detach(thread);

    pthread_create(&thread_menu, NULL, &execute_menu, NULL); 
    pthread_detach(thread_menu); 

    while(1) { 
        send_message(GET_TEMP_EVENT, strlen(GET_TEMP_EVENT));
        sleep(1);
    }

    return 0;
}

void handle_init() {
    init_socket();
    
    /* handle csv files */
    handle_file_creation();

    return;
}

void handle_close() {
    printf("\nEncerrando execução...\n");
    close_socket();
    endwin();
    exit(0);
}
