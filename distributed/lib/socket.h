#ifndef SOCKET_H
#define SOCKET_H

void* handle_socket_received_messages();
int init_socket();
void close_socket();
void send_message_to_server(char* message, int messageSize);

#endif /* SOCKET_H */