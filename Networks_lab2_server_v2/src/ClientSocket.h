//
// Created by Chase Brown on 3/22/2018.
//

#ifndef NETWORKS_LAB2_CLIENT_CLIENTSOCKET_H
#define NETWORKS_LAB2_CLIENT_CLIENTSOCKET_H
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <netdb.h>


#define PORT 3493
#define BACKLOG 10
#define Q_number 255

class ClientSocket{
private:
    int socket_fd;
    struct sockaddr_in my_addr{};
    struct sockaddr_in their_addr{};
    char buf[255];
    char retbuf[255];
    socklen_t len;
    socklen_t rlen;
    hostent *hp;
public:
    ClientSocket();

    explicit ClientSocket(int fd);
    ~ClientSocket();
    int get_socket_fd();
    int Send_Message(char* message, int message_size);
    int Start_Server(int port, int address);
    ClientSocket* Accept_Connection(struct sockaddr_in & their_addr);
    int Connect_Server(int port, char* name);
    int Receive_Message(char* buffer, int buffer_size);
    int CloseSocket();
};

enum message_type{
    CONNECT_OK=1, TRACK=10, SUCCESS=30, FAILURE=40, QUIT=50
};
enum header_type{
    GET_USER = 60, SEND_USER = 70, GET_PASSWORD = 80,
    SEND_PASSWORD = 90, LOGIN = 100, REGISTER = 200,
    MENU = 150, APPOINTMENTS = 160, EDIT_USER = 170, DELETE_USER = 180,
    NEW_APPOINTMENT = 190, VIEW_APPOINTMENTS = 210, MENU2 = 220,
    UPDATE_APPS = 230, SAVE_UPDATE_APPS = 240, GOOD = 250
};

struct message{
    message_type type;
    header_type header;
    char payload[255];
};

#endif //NETWORKS_LAB2_CLIENT_CLIENTSOCKET_H
