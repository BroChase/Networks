#include <cstdio>
#include <cstdlib>
#include <string>
#include "ClientSocket.h"
#include "passwordfunctions.h"
#include "users.h"

int main(int argc, char *argv[]) {
    int numbytes;
    //char sendbuf[MAXDATA];
    char* recvbuf;
    char* temp;
    std::vector<appointment> loguserapps;
    user sentuser;

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    auto * Client_Socket = new ClientSocket();
    Client_Socket->Connect_Server(PORT,argv[1]);
    for(;;){
        message incoming_message{};
        message outgoing_message{};
        char sendbuf[MAXDATA];
        Client_Socket->Receive_Message((char*)&incoming_message,sizeof(incoming_message));
        switch(incoming_message.type){
            case CONNECT_OK:{
                printf("Connection to server established\n");
                break;
            }
            case SUCCESS:{
                switch(incoming_message.header){
                    case GET_USER:{
                        //User name is requested from the server.
                        printf("User Name:\n");
                        gets(sendbuf);
                        numbytes=sizeof(sendbuf);
                        sendbuf[numbytes]='\0';
                        memcpy((void*)outgoing_message.payload, (void*)&sendbuf, static_cast<size_t>(numbytes));
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = SEND_USER;
                        Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case GET_PASSWORD:{
                        //Password request from server.
                        //User input 'password'
                        //password is encrypted with the random key sent from the server
                        //encrypted string is sent back to the server to be decrpyted.
                        printf("Password:\n");
                        recvbuf = incoming_message.payload;
                        gets(sendbuf);
                        numbytes=sizeof(sendbuf);
                        sendbuf[numbytes]='\0';
                        std::string key(recvbuf);
                        std::string encrypt_message = encrypt(sendbuf,recvbuf);
                        temp = (char*)calloc(128, sizeof(char));
                        strncpy(temp, encrypt_message.c_str(), 128);
                        outgoing_message.type = SUCCESS;
                        outgoing_message.header = SEND_PASSWORD;
                        memcpy((void*)outgoing_message.payload, (void*)temp, key.length());
                        Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case LOGIN:{
                        //CLIENT SUCCESSFULLY LOGGED IN
                        //SERVER SENDS CLIENT USER INFO
                        recvbuf = incoming_message.payload;
                        recvbuf = strtok(recvbuf,",");
                        sentuser.setName(recvbuf);
                        recvbuf = strtok(nullptr,",");
                        sentuser.setPassword(recvbuf);
                        recvbuf = strtok(nullptr,",");
                        sentuser.setPhone(recvbuf);
                        recvbuf = strtok(nullptr, ",");
                        sentuser.setEmail(recvbuf);
                        bool con = true;
                        while(con){
                            printf("Logged In:\nView Appointments[1]\nEdit User[2]\nDelete User[3]\nExit[4]\n");
                            gets(sendbuf);
                            numbytes=sizeof(sendbuf);
                            sendbuf[numbytes]='\0';
                            int n = atoi(sendbuf);
                            switch(n){
                                case 1:{
                                    //SEND SERVER REQUEST TO VIEW APPOINTMENTS
                                    //todo testing clear()
                                    loguserapps.clear();
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = APPOINTMENTS;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 2:{
                                    //SEND SERVER UPDATED USER INFO
                                    edit_user(sentuser);
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = EDIT_USER;
                                    std::string str = sentuser.getName() + "," + sentuser.getPassword() + "," + sentuser.getPhone() + "," + sentuser.getEmail() + ",";
                                    temp=(char*)calloc(128,sizeof(char));
                                    strncpy(temp, str.c_str(),128);
                                    memcpy((void*)outgoing_message.payload, (void*)temp, str.length());
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    printf("User Updated\n");
                                    break;
                                }
                                case 3:{
                                    //SEND SERVER REQUEST TO DELETE USER FROM SERVER.
                                    temp= (char*)calloc(128, sizeof(char));
                                    strncpy(temp, sentuser.getName().c_str(),128);
                                    while(true){
                                        printf("Confirm Delete user %s Confirm[1]\nAbort\n", temp);
                                        gets(sendbuf);
                                        int n = atoi(sendbuf);
                                        switch(n){
                                            case 1:{
                                                outgoing_message.type = SUCCESS;
                                                outgoing_message.header = DELETE_USER;
                                                Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                                printf("User Deleted Exiting\n");
                                                //if the user is deleted the user is logged off the system.
                                                return 0;
                                            }
                                            case 2:{
                                                printf("Abort\n");
                                                break;
                                            }
                                            default:{
                                                continue;
                                            }
                                        }break;
                                    }break;
                                }
                                case 4:{
                                    //DISCONNECT CLIENT FROM SERVER
                                    printf("Disconnecting\n");
                                    outgoing_message.type = QUIT;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    return 0;
                                }
                                default:{
                                    printf("Bad Input\n");
                                    continue;
                                }
                            }con = false;
                        }break;
                    }
                    case MENU:{
                        bool con = true;
                        while(con){
                            printf("Login [1]\nRegister[2]\nExit[3]\n");
                            gets(sendbuf);
                            numbytes=sizeof(sendbuf);
                            sendbuf[numbytes]='\0';
                            int n = atoi(sendbuf);
                            switch(n){
                                case 1:{
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = LOGIN;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 2:{
                                    user newuser;
                                    create_user(newuser);
                                    std::string str = newuser.getName() + "," + newuser.getPassword() + "," + newuser.getPhone() + "," + newuser.getEmail() + ",";
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = REGISTER;
                                    temp=(char*)calloc(128,sizeof(char));
                                    strncpy(temp, str.c_str(),128);
                                    memcpy((void*)outgoing_message.payload, (void*)temp, str.length());
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 3:{
                                    printf("Disconnecting\n");
                                    outgoing_message.type = QUIT;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    return 0;
                                }
                                default:{
                                    printf("Bad Input\n");
                                    continue;
                                }
                            }con = false;
                        }break;
                    }
                    case APPOINTMENTS:{
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
                        loguserapps.push_back(new_app);
                        outgoing_message.type = TRACK;
                        outgoing_message.header = GOOD;
                        Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case VIEW_APPOINTMENTS: {
                        //VIEW THE APPOINTMENTS WITH THE OPTIONS TO ADD,FILTER,EDIT,DELETE,RETURN TO PREV MENU
                        int count = 0;
                        for (auto value: loguserapps) {
                            std::string app;
                            temp = (char *) calloc(512, sizeof(char));
                            app = "[" + std::to_string(count) + "]   " + value.get_start_time() + " " +
                                  value.get_start_date() + " " +
                                  value.get_end_time() + " " + value.get_end_date() + "\n" +
                                  value.get_place() + "\n" + value.get_app_details()+"\n";
                            strncpy(temp, app.c_str(), 512);
                            printf("%s\n", temp);
                            count += 1;
                        }
                        bool con = true;
                        while(con){
                            printf("Add Appointment[1]\nFilter Appointments[2]\nEdit An Appointment[3]\n"
                                           "Delete An Appointment[4]\nReturn to Previous Menu[5]\n\n");
                            gets(sendbuf);
                            numbytes=sizeof(sendbuf);
                            sendbuf[numbytes]='\0';
                            int n = atoi(sendbuf);
                            switch(n) {
                                case 1: {
                                    printf("Add Appointment\n");
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = NEW_APPOINTMENT;
                                    std::string appointment = new_appointment();
                                    temp = (char *) calloc(128, sizeof(char));
                                    strncpy(temp, appointment.c_str(), 128);
                                    std::string place, date, dummie, time;
                                    time = strtok(temp, ",");
                                    date = strtok(nullptr, ",");
                                    dummie = strtok(nullptr, ",");
                                    dummie = strtok(nullptr, ",");
                                    place = strtok(nullptr, ",");
                                    //CHECK AGAINST CURRENT USER APPOINTMENTS TO CONFIRM NO DUPS.
                                    for (auto value: loguserapps) {
                                        if (value.get_place() == place && value.get_start_time() == time &&
                                            value.get_start_date() == date) {
                                            printf("Duplicate");
                                            break;
                                        }
                                    }
                                    temp = (char *) calloc(512, sizeof(char));
                                    strncpy(temp, appointment.c_str(), 512);
                                    memcpy((void *) outgoing_message.payload, (void *) temp, appointment.length());
                                    Client_Socket->Send_Message((char *) &outgoing_message, sizeof(outgoing_message));
                                    con = false;
                                    break;
                                }
                                case 2: {
                                    printf("Filter Appointments\n");
                                    filter_apps(loguserapps);
                                    outgoing_message.type = TRACK;
                                    outgoing_message.header = APPOINTMENTS;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 3: {//todo work out bugs
                                    printf("Edit Appointment\n");
                                    ed_apps(loguserapps);
                                    for(auto value: loguserapps){
                                        std::string app;
                                        app = value.get_start_time()+","+value.get_start_date()+","+
                                              value.get_end_time()+","+value.get_end_date()+","+value.get_place()+","+value.get_app_details()+",";
                                        temp=(char*)calloc(512,sizeof(char));
                                        strncpy(temp, app.c_str(), 512);
                                        memcpy((void*)outgoing_message.payload, (void*)temp, app.length());
                                        outgoing_message.type = SUCCESS;
                                        outgoing_message.header = UPDATE_APPS;
                                        Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    }
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = SAVE_UPDATE_APPS;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 4: {//todo work out bugs
                                    printf("Delete Appointment\n");
                                    del_apps(loguserapps);
                                    for(auto value: loguserapps){
                                        std::string app;
                                        app = value.get_start_time()+","+value.get_start_date()+","+
                                              value.get_end_time()+","+value.get_end_date()+","+value.get_place()+","+value.get_app_details()+",";
                                        temp=(char*)calloc(512,sizeof(char));
                                        strncpy(temp, app.c_str(), 512);
                                        memcpy((void*)outgoing_message.payload, (void*)temp, app.length());
                                        outgoing_message.type = SUCCESS;
                                        outgoing_message.header = UPDATE_APPS;
                                        Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    }
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = SAVE_UPDATE_APPS;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 5: {
                                    con = false;
                                    break;
                                }
                                default: {
                                    continue;
                                }
                            }
                        }
                        outgoing_message.type = TRACK;
                        outgoing_message.header = APPOINTMENTS;
                        Client_Socket->Send_Message((char *) &outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                    case MENU2:{
                        bool con = true;
                        while(con){
                            printf("View Appointments[1]\nEdit User[2]\nDelete User[3]\nExit[4]\n");
                            gets(sendbuf);
                            numbytes=sizeof(sendbuf);
                            sendbuf[numbytes]='\0';
                            int n = atoi(sendbuf);
                            switch(n){
                                case 1:{
                                    //todo testing clear();
                                    loguserapps.clear();
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = APPOINTMENTS;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 2:{
                                    edit_user(sentuser);
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = EDIT_USER;
                                    std::string str = sentuser.getName() + "," + sentuser.getPassword() + "," + sentuser.getPhone() + "," + sentuser.getEmail() + ",";
                                    temp=(char*)calloc(128,sizeof(char));
                                    strncpy(temp, str.c_str(),128);
                                    memcpy((void*)outgoing_message.payload, (void*)temp, str.length());
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    printf("User Updated\n");
                                    break;
                                }
                                case 3:{
                                    temp= (char*)calloc(128, sizeof(char));
                                    strncpy(temp, sentuser.getName().c_str(),128);
                                    while(true){
                                        printf("Confirm Delete user %s Confirm[1]\nAbort[2]\n", temp);
                                        gets(sendbuf);
                                        int n = atoi(sendbuf);
                                        switch(n){
                                            case 1:{
                                                //USER IS DELETED AND LOGGED OFF THE SYSTEM
                                                outgoing_message.type = SUCCESS;
                                                outgoing_message.header = DELETE_USER;
                                                Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                                printf("User Deleted Exiting\n");
                                                return 0;
                                            }
                                            case 2:{
                                                printf("Abort\n");
                                                break;
                                            }
                                            default:{
                                                continue;
                                            }
                                        }break;
                                    }break;
                                }
                                case 4:{
                                    //DISCONNECT FROM SERVER
                                    printf("Disconnecting\n");
                                    outgoing_message.type = QUIT;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    return 0;
                                }
                                default:{
                                    printf("Bad Input\n");
                                    continue;
                                }
                            }con = false;
                        }break;
                    }
                    case NEW_APPOINTMENT:{
                        //CONFIRMATION THAT APPOINTMENT WAS SAVED
                        printf("Appointment Saved!\n");
                        outgoing_message.type = TRACK;
                        outgoing_message.header = APPOINTMENTS;
                        Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                        break;
                    }
                }break;
            }
            case FAILURE:{
                switch(incoming_message.header){
                    case REGISTER:{
                        //SERVER FAILED TO REGISTER USER
                        printf("User Registration Failed\n");
                        outgoing_message.type = TRACK;
                        outgoing_message.header = MENU;
                        Client_Socket->Send_Message((char*)&outgoing_message,sizeof(outgoing_message));
                        break;
                    }
                    case APPOINTMENTS:{
                        //SERVER FOUND NO APPOINTMENTS FOR USER
                        bool con = true;
                        while(con){
                            printf("No Appointments\nAdd Appointments[1]\nReturn to Login Menu[2]\n");
                            gets(sendbuf);
                            numbytes=sizeof(sendbuf);
                            sendbuf[numbytes]='\0';
                            int n = atoi(sendbuf);
                            switch(n){
                                case 1:{
                                    //ADD AN APPOINTMENT FOR USER
                                    outgoing_message.type = SUCCESS;
                                    outgoing_message.header = NEW_APPOINTMENT;
                                    std::string appointment = new_appointment();
                                    temp=(char*)calloc(512, sizeof(char));
                                    strncpy(temp, appointment.c_str(), 255);
                                    memcpy((void*)outgoing_message.payload,(void*)temp, appointment.length());
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }
                                case 2:{
                                    //DONT ADD NEW APPOINTMENT RETURN TO MENU
                                    outgoing_message.type = TRACK;
                                    outgoing_message.header = APPOINTMENTS;
                                    Client_Socket->Send_Message((char*)&outgoing_message, sizeof(outgoing_message));
                                    break;
                                }default:{
                                    continue;
                                }
                            }con = false;
                        }break;
                    }
                    default:break;
                }break;
            }
            case QUIT:{
                return 0;
            }
        }
    }
    return 0;
}