//
// Created by Chase Brown on 3/21/2018.
//
#include <fstream>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <cstring>
#include "fileops.h"
#include "ClientSocket.h"

std::vector<appointment> get_user_appointments(std::string name){
    std::vector<appointment>  appointments;
    appointment newappointment;
    std::ifstream appfile;
    //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+ name + "appoint.csv";
    std::string file = "./Users/"+name + "appoint.csv";
    appfile.open(file);
    if(appfile.good()){
        std::string line;
        while(std::getline(appfile, line)){
            newappointment.set_start_time(line);
            getline(appfile, line);
            newappointment.set_start_date(line);
            getline(appfile, line);
            newappointment.set_end_time(line);
            getline(appfile, line);
            newappointment.set_end_date(line);
            getline(appfile, line);
            newappointment.setPlace(line);
            getline(appfile, line);
            newappointment.set_app_details(line);
            //get dummy line
            getline(appfile, line);
            appointments.push_back(newappointment);
        }
    }
    return appointments;
}
void save_updated_appointments(std::string name, std::vector<appointment> apps){
    //std:: string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+name+"appoint.csv";
    std:: string file = "./Users/"+name+"appoint.csv";
    std::ofstream appfile;
    appfile.open(file);
    for(auto value: apps){
        appfile << value.get_start_time()+"\n";
        appfile << value.get_start_date()+"\n";
        appfile << value.get_end_time()+"\n";
        appfile << value.get_end_date()+"\n";
        appfile << value.get_place()+"\n";
        appfile << value.get_app_details()+"\n";
        appfile << "|\n";
    }
    appfile.close();
}
void save_appointment(std::string name, appointment newappointment){
    std::ofstream appfile;
    //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+ name + "appoint.csv";
    std::string file = "./Users/"+name + "appoint.csv";
    appfile.open(file, std::ios_base::app);
    appfile << newappointment.get_start_time()+"\n";
    appfile << newappointment.get_start_date()+"\n";
    appfile << newappointment.get_end_time()+"\n";
    appfile << newappointment.get_end_date()+"\n";
    appfile << newappointment.get_place()+"\n";
    appfile << newappointment.get_app_details()+"\n";
    appfile << "|\n";
    appfile.close();
}

void save_new_user(user newuser){
    std::ofstream userfile, userfile2;
    //create a user
    //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+newuser.getName() + ".csv";
    std::string file = "./Users/"+newuser.getName() + ".csv";
    //create a user appointment file
    //std::string file2 = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+newuser.getName() + "appoint.csv";
    std::string file2 = "./Users/"+newuser.getName() + "appoint.csv";
    userfile2.open(file2);
    userfile2.close();
    userfile.open(file);//, std::ios_base::app);
    userfile << newuser.getName()+"\n";
    userfile << newuser.getPassword()+"\n";
    userfile << newuser.getPhone()+"\n";
    userfile << newuser.getEmail()+"\n";
    userfile << "|\n";
    userfile.close();
}


bool look_up_users(std::string chk, user& serveruser) {
    char *temp;
    std::string place, s_t, e_t, s_d, e_d, s_a_d;
    std::ifstream userfile;
    //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/" + chk + ".csv";
    std::string file = "./Users/"+chk + ".csv";
    userfile.open(file);
    if (userfile.good()) {
        std::string line;
        while (std::getline(userfile, line)) {
            temp = (char *) calloc(128, sizeof(char));
            strncpy(temp, line.c_str(), 128);
            serveruser.setName(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(userfile, line);
            strncpy(temp, line.c_str(), 128);
            serveruser.setPassword(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(userfile, line);
            strncpy(temp, line.c_str(), 128);
            serveruser.setPhone(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(userfile, line);
            strncpy(temp, line.c_str(), 128);
            serveruser.setEmail(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(userfile, line);
            strncpy(temp, line.c_str(), 128);
            return true;
        }
    } else
        return false;
}

void load_appointments(std::vector<appointment> &loguserapp, std::string name) {
    std::ifstream appfile;
    appointment app;
    char* temp;
    //std::string file = "C:/Users/Chase Brown/ClionProjects/Networks_lab2_server/Users/"+ name + "appoint.csv";
    std::string file = "./Users/"+name + "appoint.csv";
    appfile.open(file);
    if(appfile.good()){
        std::string line;
        while(std::getline(appfile,line)){
            temp = (char *) calloc(128, sizeof(char));
            strncpy(temp, line.c_str(), 128);
            app.set_start_time(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(appfile, line);
            strncpy(temp, line.c_str(), 128);
            app.set_start_date(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(appfile, line);
            strncpy(temp, line.c_str(), 128);
            app.set_end_time(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(appfile, line);
            strncpy(temp, line.c_str(), 128);
            app.set_end_date(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(appfile, line);
            strncpy(temp, line.c_str(), 128);
            app.setPlace(temp);

            temp = (char *) calloc(128, sizeof(char));
            std::getline(appfile, line);
            strncpy(temp, line.c_str(), 128);
            app.set_app_details(temp);
            //dummyline
            std::getline(appfile, line);
            loguserapp.push_back(app);
        }
    }
}
