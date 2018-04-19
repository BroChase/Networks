//
// Created by Chase Brown on 3/22/2018.
//

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include "ClientSocket.h"

ClientSocket::ClientSocket() {
    len = sizeof(my_addr);
    rlen = sizeof(their_addr);
}
/***************
 * Creates a socket with file descriptor
 * @param fd
 */
ClientSocket::ClientSocket(int fd) {
    socket_fd = fd;
    len = sizeof(my_addr);
    rlen = sizeof(their_addr);
}

ClientSocket::~ClientSocket() {
    this->CloseSocket();
}
/*********************************
 * Send message over socket
 * @param message
 * @param message_size
 * @return int
 */
int ClientSocket::Send_Message(char *message, int message_size) {
    return static_cast<int>(send(socket_fd, message, static_cast<size_t>(message_size), 0));
}
/***********************************
 * Receive message over tcp
 * @param buffer
 * @param buffer_size
 * @return
 */
int ClientSocket::Receive_Message(char *buffer, int buffer_size) {

    auto messagelen = static_cast<int>(recv(socket_fd, buffer, static_cast<size_t>(buffer_size), 0));
    return messagelen;
}
/******************************
 * Start TCP server
 * @param port
 * @param address
 * @return
 */
int ClientSocket::Start_Server(int port, int address) {
    int sockfd;
    int yes = 1;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    //Creates a socket
    struct sockaddr_in my_addr{};
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    //Socket Settings
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(PORT);
    memset(&(my_addr.sin_zero), '\0', 8);

    //bind() socket to address
    if(bind(socket_fd, (struct sockaddr *)&my_addr, len) == -1){
        perror("bind");
        exit(1);
    }
    if(listen(socket_fd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    getsockname(socket_fd, (struct sockaddr*)&my_addr, &len);
    return my_addr.sin_port;
}
/****************
 * Accept the connection and create a socket
 * @return new connection socket
 */
ClientSocket *ClientSocket::Accept_Connection() {
    ClientSocket* new_socket = nullptr;
    int accept_connection_fd = accept(socket_fd, (struct sockaddr*)0,(socklen_t*)0);
    if(accept_connection_fd != -1){
        new_socket = new ClientSocket(accept_connection_fd);
    }
    return new_socket;
}

int ClientSocket::Connect_Server(int port, char *name) {
    //create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    //Socket Settings
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    hp = gethostbyname(name);
    memcpy(&their_addr.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));


    //connect
    if(connect(socket_fd, (struct sockaddr*)&their_addr, sizeof(their_addr)) < 0){
        return -1;
        close(socket_fd);
    }
    return 0;
}

/******************
 * Close socket
 * @return
 */
int ClientSocket::CloseSocket() {
    close(socket_fd);
    return 0;
}