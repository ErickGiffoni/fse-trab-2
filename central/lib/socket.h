#ifndef SOCKET_H
#define SOCKET_H

extern volatile int connected;

void handle_messages();
void send_message_to_server(char* message, int messageSize);
int init_socket();
void send_message(char* message, int messageSize);
void* wait_client();
void close_socket();

#endif /* SOCKET_H */