#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "ClientSocket.h"
#include "users.h"
#include "passwordfunctions.h"
#include "fileops.h"


void *connection_handler(void*);

int main() {
    struct sockaddr_in their_addr{};
    int *new_sock;

    //START SERVER LISTENING
    auto* server = new ClientSocket();

    server->Start_Server(PORT, INADDR_ANY);

    printf("%i\n", server->get_socket_fd());
    //ACCEPT A NEW CONNECTION AND SEND A CONNECTION OK TO CLIENT
    while(ClientSocket* pc_connection = server->Accept_Connection(their_addr)){
        pthread_t client_thread;
        new_sock = static_cast<int *>(malloc(sizeof(*new_sock)));
        *new_sock = pc_connection->get_socket_fd();
        //CREATE A NEW THREAD FOR NEW CONNECTION
        if(pthread_create(&client_thread, nullptr, connection_handler, pc_connection)){
            perror("unable to create thread");
            return 1;
        }
    }

    return 0;
}
//THREAD HANDLER
void *connection_handler(void* pc_c){
    auto * pc_connection = (ClientSocket*)pc_c;
    message outgoing_message{};
    message incoming_message{};
    std::vector<appointment> loguserapps;
    std::vector<appointment> updatedappointments;
    struct sockaddr_in their_addr{};
    char* recvbuf;
    char* temp;
    std::string tempstring;
    bool set;
    user loguser;// HOLD LOGGED IN USER

    //SEND A MESSAGE TO THE CLIENT THEIR CONNECTION IS OK
    outgoing_message.type = CONNECT_OK;
    pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
    printf("Server: got connection %s\n", inet_ntoa(their_addr.sin_addr));
    outgoing_message.type = SUCCESS;
    outgoing_message.header = MENU;
    pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
    //START INFINITE LOOP 'STEAM'
    for(;;){
        pc_connection->Receive_Message((char*)&incoming_message, sizeof(incoming_message));
        switch(incoming_message.type){
            case CONNECT_OK:{
                printf("Connection to server established\n");
                break;
            }
            case SUCCESS:{
                switch(incoming_message.header){
                    case LOGIN:{ //LOGIN REQUESTED BY USER
                        printf("Login Requested:\n");
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = GET_USER;
                        pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case SEND_USER:{
                        //USER INPUT FROM CLIENT
                        set = false;
                        while(!set){//CHECKS TO SEE IF THE USER EXISTS IF NOT REQUEST FOR USER IS SENT BACK
                            recvbuf = incoming_message.payload;
                            printf("User: %s\n", recvbuf);
                            std::string str(recvbuf);
                            set = look_up_users(str, loguser);
                            if(!set){
                                outgoing_message.type = SUCCESS;
                                outgoing_message.header = MENU;
                                pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                pc_connection->Receive_Message((char*)&incoming_message, sizeof(incoming_message));
                            } else
                                continue;
                        }
                        //IF THE USER NAME IS GOOD GET PASSWORD FROM CLIENT
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = GET_PASSWORD;
                        size_t passwordlength = static_cast<int>(loguser.getPassword().length());
                        //CREATES A RANDOME STRING SENT TO THE USER
                        std::string s = random_string(passwordlength);
                        tempstring = s;
                        temp = (char*)calloc(128, sizeof(char));
                        strncpy(temp, s.c_str(),128);
                        memcpy((void*)outgoing_message.payload,(void*)temp, passwordlength);
                        pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case SEND_PASSWORD:{
                        //Client ENCRYPTED STRING IS SENT BACK
                        recvbuf = incoming_message.payload;
                        std::string userinput(recvbuf);
                        //DECRYPT CLIENT STRING AND COMPARE WITH STORED  USER PASSWORD
                        std::string pwd = decrypt(tempstring, userinput);
                        if(pwd == loguser.getPassword()){
                            outgoing_message.type = SUCCESS;
                            outgoing_message.header = LOGIN;
                            std::string str = loguser.getName() + "," + loguser.getPassword() + "," + loguser.getPhone() + "," + loguser.getEmail() + ",";
                            temp = (char*)calloc(128,sizeof(char));
                            strncpy(temp, str.c_str(), 128);
                            memcpy((void*)outgoing_message.payload, (void*)temp, str.length());
                            pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        } else{
                            outgoing_message.type = SUCCESS;
                            outgoing_message.header = GET_PASSWORD;
                            size_t passwordlength = static_cast<int>(loguser.getPassword().length());
                            std::string s = random_string(passwordlength);
                            tempstring = s;
                            temp = (char*)calloc(128, sizeof(char));
                            strncpy(temp, s.c_str(),128);
                            memcpy((void*)outgoing_message.payload,(void*)temp, passwordlength);
                            pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        }
                        break;
                    }
                    case REGISTER:{//CLIENT REQUESTS TO REGISTER NEW USER
                        std::string name, password, phone, email;
                        printf("Registration Requested:\n");
                        user sentuser;
                        recvbuf = incoming_message.payload;
                        recvbuf = strtok(recvbuf,",");
                        sentuser.setName(recvbuf);
                        recvbuf = strtok(nullptr,",");
                        sentuser.setPassword(recvbuf);
                        recvbuf = strtok(nullptr,",");
                        sentuser.setPhone(recvbuf);
                        recvbuf = strtok(nullptr, ",");
                        sentuser.setEmail(recvbuf);
                        set = look_up_users(sentuser.getName(), sentuser);
                        //IF USER ALREADY EXISTS FAILS ELSE SAVE NEW USER INFO
                        if(set){
                            outgoing_message.type = FAILURE;
                            outgoing_message.header = REGISTER;
                            pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        }else{
                            save_new_user(sentuser);
                            outgoing_message.type = SUCCESS;
                            outgoing_message.header = MENU;
                            pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        }
                        break;
                    }
                    case APPOINTMENTS:{
                        //CLIENT HAS REQUESTED THE APPOINTMENTS FOR USER
                        //std::vector<appointment> userappointments;
                        printf("Appointments Requested:\n");
                        //todo testing clear()
                        loguserapps.clear();
                        loguserapps = get_user_appointments(loguser.getName());
                        //IF NO APPOINTMENTS EXIST CLIENT IS INFORMED
                        if(loguserapps.empty()){
                            outgoing_message.type = FAILURE;
                            outgoing_message.header = APPOINTMENTS;
                            pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        } else{
                            //SEND APPOINTMENTS FOR CLIENT
                            //todo testing clear();
                            loguserapps.clear();
                            load_appointments(loguserapps, loguser.getName());
                            for(auto value: loguserapps){
                                std::string app;
                                app = value.get_start_time()+","+value.get_start_date()+","+
                                      value.get_end_time()+","+value.get_end_date()+","+value.get_place()+","+value.get_app_details()+",";
                                temp=(char*)calloc(512,sizeof(char));
                                strncpy(temp, app.c_str(), 512);
                                memcpy((void*)outgoing_message.payload, (void*)temp, app.length());
                                outgoing_message.type = SUCCESS;
                                outgoing_message.header = APPOINTMENTS;
                                pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                            }
                            outgoing_message.type = SUCCESS;
                            outgoing_message.header = VIEW_APPOINTMENTS;
                            pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        }
                        break;
                    }
                    case UPDATE_APPS:{
                        std::string stime, sdate, etime, edate, p, ad;
                        appointment new_app;
                        recvbuf = incoming_message.payload;
                        stime = strtok(recvbuf,",");
                        sdate = strtok(nullptr,",");
                        etime = strtok(nullptr,",");
                        edate = strtok(nullptr, ",");
                        p = strtok(nullptr, ",");
                        ad = strtok(nullptr, ",");
                        new_app.set_start_time(stime);
                        new_app.set_start_date(sdate);
                        new_app.set_end_time(etime);
                        new_app.set_end_date(edate);
                        new_app.setPlace(p);
                        new_app.set_app_details(ad);
                        updatedappointments.push_back(new_app);
                        break;
                    }
                    case SAVE_UPDATE_APPS:{
                        //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+loguser.getName()+"appoint.csv";
                        std::string file = "./Users/"+loguser.getName()+"appoint.csv";
                        temp=(char*)calloc(128,sizeof(char));
                        strncpy(temp, file.c_str(),128);
                        std::remove(temp);
                        save_updated_appointments(loguser.getName(), updatedappointments);

                        break;
                    }
                    case EDIT_USER:{ //RECEIVE UPDATED USER AND OVERRIDE CURRENT USER INTO
                        char* temp2;
                        user sentuser;
                        recvbuf = incoming_message.payload;
                        recvbuf = strtok(recvbuf,",");
                        sentuser.setName(recvbuf);
                        recvbuf = strtok(nullptr,",");
                        sentuser.setPassword(recvbuf);
                        recvbuf = strtok(nullptr,",");
                        sentuser.setPhone(recvbuf);
                        recvbuf = strtok(nullptr, ",");
                        sentuser.setEmail(recvbuf);
                        //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/" + loguser.getName() + ".csv";
                        std::string file = "./Users/"+loguser.getName() + ".csv";
                        temp = (char*)calloc(128,sizeof(char));
                        strncpy(temp, file.c_str(),128);
                        std::remove(temp);
                        save_new_user(sentuser);
                        file = "./Users/"+loguser.getName() + "appoint.csv";
                        temp = (char*)calloc(128,sizeof(char));
                        strncpy(temp, file.c_str(), 128);
                        std::string file2 = "./users/"+sentuser.getName() + "appoint.csv";
                        temp2 = (char*)calloc(128, sizeof(char));
                        strncpy(temp2, file2.c_str(), 128);
                        std::rename(temp, temp2);


                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = LOGIN;
                        std::string str = sentuser.getName() + "," + sentuser.getPassword() + "," + sentuser.getPhone() + "," + sentuser.getEmail() + ",";
                        temp = (char*)calloc(128,sizeof(char));
                        strncpy(temp, str.c_str(), 128);
                        memcpy((void*)outgoing_message.payload, (void*)temp, str.length());
                        pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case DELETE_USER:{
                        //REMOVAL OF USER AND THEIR APPOINTMENTS
                        //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+loguser.getName()+".csv";
                        std::string file = "./Users/"+loguser.getName()+".csv";
                        temp = (char*)calloc(128,sizeof(char));
                        strncpy(temp, file.c_str(),128);
                        std::remove(temp);
                        //file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+loguser.getName()+"appoint.csv";
                        file = "./Users/"+loguser.getName()+"appoint.csv";
                        temp=(char*)calloc(128,sizeof(char));
                        strncpy(temp, file.c_str(),128);
                        std::remove(temp);
                        pc_connection->CloseSocket();
                        break;
                    }
                    case VIEW_APPOINTMENTS:{
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = VIEW_APPOINTMENTS;
                        pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case NEW_APPOINTMENT:{
                        //APPEND NEW APPOINTMENT TO THE USER APPOINTMENTS
                        std::string stime, sdate, etime, edate, p, ad;
                        printf("Adding New Appointment:\n");
                        appointment new_app;
                        recvbuf = incoming_message.payload;
                        stime = strtok(recvbuf,",");
                        sdate = strtok(nullptr,",");
                        etime = strtok(nullptr,",");
                        edate = strtok(nullptr, ",");
                        p = strtok(nullptr, ",");
                        ad = strtok(nullptr, ",");
                        new_app.set_start_time(stime);
                        new_app.set_start_date(sdate);
                        new_app.set_end_time(etime);
                        new_app.set_end_date(edate);
                        new_app.setPlace(p);
                        new_app.set_app_details(ad);
                        save_appointment(loguser.getName(), new_app);
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = NEW_APPOINTMENT;
                        pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                }
                break;
            }
            case TRACK:{
                switch(incoming_message.header){
                    case MENU:{
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = MENU;
                        pc_connection->Send_Message((char*)&outgoing_message,sizeof(outgoing_message));
                        break;
                    }
                    case APPOINTMENTS:{
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = MENU2;
                        pc_connection->Send_Message((char*)&outgoing_message,sizeof(outgoing_message));
                        break;
                    }
                    case VIEW_APPOINTMENTS:{
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = MENU2;
                        pc_connection->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case GOOD:{
                        printf("Ok:\n");
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case QUIT:{
                printf("Client Disconnected\n");
                pc_connection->CloseSocket();
                return 0;
            }
        }
    }
}
