//
// Created by Chase Brown on 3/20/2018.
//
#include <iostream>
//#include <regex>
#include <algorithm>
#include <cstring>
#include "users.h"
#include "ClientSocket.h"

void create_user(user& newuser){
    bool set = false;
    char sendbuf[MAXDATA];
    int numbytes;
    while(!set){
        printf("Enter Name:\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = newuser.setName(sendbuf);
    }
    set = false;
    while(!set){
        printf("Enter Password(8-16 alphanumeric):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = newuser.setPassword(sendbuf);
    }
    set = false;
    while(!set){
        printf("Enter Phone(999999999):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = newuser.setPhone(sendbuf);
    }
    set = false;
    while(!set){
        printf("Enter Email:\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = newuser.setEmail(sendbuf);
    }
}
void edit_user(user& loguser){
    int numbytes;
    user newuser;
    char* temp;
    bool set;
    char sendbuf[MAXDATA];
    while(true){
        printf("Edit Name[1]\nEdit Password[2]\nEdit Phone[3]\nEdit Email[4]\nFinished Editing[5]\n");
        temp=(char*)calloc(128,sizeof(char));
        strncpy(temp, loguser.getName().c_str(),128);
        printf("Name: %s\n", temp);
        temp=(char*)calloc(128,sizeof(char));
        strncpy(temp, loguser.getPassword().c_str(),128);
        printf("Password: %s\n", temp);
        temp=(char*)calloc(128,sizeof(char));
        strncpy(temp, loguser.getPhone().c_str(),128);
        printf("Phone: %s\n", temp);
        temp=(char*)calloc(128,sizeof(char));
        strncpy(temp, loguser.getEmail().c_str(),128);
        printf("Email: %s\n", temp);

        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        int n = atoi(sendbuf);
        switch(n){
            case 1:{
                set = false;
                while(!set){
                    printf("New Name:\n");
                    gets(sendbuf);
                    numbytes=sizeof(sendbuf);
                    sendbuf[numbytes]='\0';
                    set = loguser.setName(sendbuf);
                }
                continue;
            }
            case 2:{
                set = false;
                while(!set){
                    printf("New Password:\n");
                    gets(sendbuf);
                    numbytes=sizeof(sendbuf);
                    sendbuf[numbytes]='\0';
                    set = loguser.setPassword(sendbuf);
                }
                continue;
            }
            case 3:{
                set = false;
                while(!set){
                    printf("New Phone:\n");
                    gets(sendbuf);
                    numbytes=sizeof(sendbuf);
                    sendbuf[numbytes]='\0';
                    set = loguser.setPhone(sendbuf);
                }
                continue;
            }
            case 4:{
                set = false;
                while(!set){
                    printf("New Name:\n");
                    gets(sendbuf);
                    numbytes=sizeof(sendbuf);
                    sendbuf[numbytes]='\0';
                    set = loguser.setEmail(sendbuf);
                }
                continue;
            }
            case 5:{
                break;
            }default:
                continue;
        }break;
    }
}

//user functions
//todo put on server setName setPassword setPhone set_start_time set_start_date set_end_time set_end_date
//todo string_is_valid password_is_valid phone_is_valid
bool user::setName(char *username) {
    std::string str(username);
    //check to see if the user name is alphanumeric dash only
    //bool contains = std::regex_match(str, std::regex("^[a-zA-Z0-9-]+$"));
    bool contains = string_is_valid(username);
    //printf("%d\n",contains);
    if(contains){
        name = str;
        return true;
    }else
        return false;
}

bool user::setPassword(char *pass_word) {
//longer than 8 shorter than 16 alpha-numeric characters
    std::string str(pass_word);
    //bool contains = std::regex_match(str, std::regex("^[a-zA-Z0-9]+$"));
    bool contains = password_is_valid(pass_word);
    auto L = static_cast<int>(strlen(pass_word));
    if(contains){
        if(L > 8 && L < 16){
            password = str;
            return true;
        }
    }else
        return false;
}

bool user::setPhone(char* ph) {
    std::string str(ph);
    //bool contains = std::regex_match(str, std::regex("^([0-9]{3})[-]([0-9]{3})[-]([0-9]{4})$"));
    bool contains = phone_id_valid(ph) && str.length() == 10;
    if(contains){
        phone = str;
        return true;
    } else
        return false;
}

bool user::setEmail(char* em) {
    std::string str(em);
    auto L = static_cast<int>(strlen(em));
    if(L > 0 && L < 40){
        email = em;
        return true;
    } else
        return false;
}

std::string user::getName() {
    return std::string(name);
}

std::string user::getPassword() {
    return std::string(password);
}

std::string user::getPhone() {
    return std::string(phone);
}

std::string user::getEmail() {
    return std::string(email);
}

void user::add_appointment(std::string p, std::string s_t, std::string e_t, std::string s_d, std::string e_d,
                           std::string s_a_d) {
    auto * temp_appointment = new appointment;
    temp_appointment->setPlace(p);
    temp_appointment->set_start_time(s_t);
    temp_appointment->set_end_time(e_t);
    temp_appointment->set_start_date(s_d);
    temp_appointment->set_end_date(e_d);
    temp_appointment->set_app_details(s_a_d);
    appointments.push_back(temp_appointment);
}

//appointment functions
bool appointment::setPlace(std::string p) {
//up to 64 characters in length
    if(p.length() <= 64){
        appPlace = p;
        return true;
    } else
        return false;
}

bool appointment::set_start_time(std::string s_t) {
//HH::MM format
    //bool contains = std::regex_match(s_t, std::regex("^([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$"));
    bool contains = check_valid_time(s_t);
    if(contains){
        startTime = s_t;
        return true;
    } else
        return false;
}

bool appointment::set_start_date(std::string s_d) {
//MM/DD/YEAR format
    //bool contains = std::regex_match(s_d, std::regex("^(0[1-9]|1[0-2])\\/(0[1-9]|1\\d|2\\d|3[01])\\/(19|20)\\d{2}$"));
    bool contains = check_valid_date(s_d);
    if(contains){
        startDate = s_d;
        return true;
    } else
        return false;
}

bool appointment::set_end_time(std::string e_t) {
//HH:MM format
    //bool contains = std::regex_match(e_t, std::regex("^([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$"));
    bool contains = check_valid_time(e_t);
    if(contains){
        endTime = e_t;
        return true;
    } else
        return false;
}

bool appointment::set_end_date(std::string e_d) {
//MM/DD/YEAR format
    //bool contains = std::regex_match(e_d, std::regex("^(0[1-9]|1[0-2])\\/(0[1-9]|1\\d|2\\d|3[01])\\/(19|20)\\d{2}$"));
    bool contains = check_valid_date(e_d);
    if(contains){
        endDate = e_d;
        return true;
    } else
        return false;
}

bool appointment::set_app_details(std::string a_d) {
//up to 128 characters in length
    if(a_d.length() <= 128){
        appDetails = a_d;
        return true;
    } else
        return false;
}

std::string appointment::get_place() {
    return std::string(appPlace);
}

std::string appointment::get_start_time() {
    return std::string(startTime);
}

std::string appointment::get_start_date() {
    return std::string(startDate);
}

std::string appointment::get_end_time() {
    return std::string(endTime);
}

std::string appointment::get_end_date() {
    return std::string(endDate);
}

std::string appointment::get_app_details() {
    return std::string(appDetails);
}
/*************
 * Create User appointment to send to server
 * @return
 */
std::string new_appointment(){
    std::string app;
    appointment appointment;
    int numbytes;
    char sendbuf[MAXDATA];
    bool set = false;

    while(!set){
        printf("Appointment Start Time-format(HH:MM):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = appointment.set_start_time(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment Start Date-format(MM/DD/YEAR):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = appointment.set_start_date(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment End Time-format(HH:MM):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = appointment.set_end_time(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment End Date-format(MM/DD/YEAR):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = appointment.set_end_date(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment Place-format(MAX 64 Characters):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = appointment.setPlace(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment Details-format(MAX 128 Characters):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = appointment.set_app_details(std::string(sendbuf));
    }
    app = appointment.get_start_time()+","+appointment.get_start_date()+","+appointment.get_end_time()+","+
            appointment.get_end_date()+","+appointment.get_place()+","+appointment.get_app_details()+",";
    return app;
}
/**************
 * Edit user appointments
 * @param logapps
 */
void ed_apps(std::vector<appointment> & logapps){
    int count = 0;
    char* temp;

    printf("Select Appointment to edit\n");
    for (auto value: logapps) {
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
    int numbytes;
    char sendbuf[MAXDATA];
    bool set = false;
    printf("Appointment Selection[enter number]\n");
    gets(sendbuf);
    numbytes = sizeof(sendbuf);
    sendbuf[numbytes] = '\0';
    int edit = atoi(sendbuf);

    while(!set){
        printf("Appointment Start Time-format(HH:MM):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = logapps[edit].set_start_time(sendbuf);
    }
    set = false;
    while(!set){
        printf("Appointment Start Date-format(MM/DD/YEAR):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = logapps[edit].set_start_date(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment End Time-format(HH:MM):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = logapps[edit].set_end_time(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment End Date-format(MM/DD/YEAR):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = logapps[edit].set_end_date(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment Place-format(MAX 64 Characters):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = logapps[edit].setPlace(std::string(sendbuf));
    }
    set = false;
    while(!set){
        printf("Appointment Details-format(MAX 128 Characters):\n");
        gets(sendbuf);
        numbytes=sizeof(sendbuf);
        sendbuf[numbytes]='\0';
        set = logapps[edit].set_app_details(std::string(sendbuf));
    }
}
/***************************
 * Delete a user appointment
 * @param logapps
 */
void del_apps(std::vector<appointment> & logapps){
    int count = 0;
    char* temp;

    printf("Select Appointment to delete\n");
    for (auto value: logapps) {
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
    printf("Appointment Selection[enter number]\n");
    int numbytes;
    char sendbuf[MAXDATA];

    gets(sendbuf);
    numbytes = sizeof(sendbuf);
    sendbuf[numbytes] = '\0';
    int edit = atoi(sendbuf);
    logapps.erase(logapps.begin()+edit);
}
/**************************
 * Check for a Valid Date Range
 * @param mm user input min month
 * @param dd user input min day
 * @param yyyy user input min year
 * @param MM user input max month
 * @param DD user input max day
 * @param YYYY user input max year
 * @param AppMM appointment month
 * @param AppDD  appointment day
 * @param AppYYYY appointment year
 * @return
 */
bool isValidDate(int mm, int dd, int yyyy, int MM, int DD, int YYYY, int AppMM, int AppDD, int AppYYYY){
    if(yyyy < AppYYYY && YYYY > AppYYYY){
        return true;
    }else if(yyyy == AppYYYY || AppYYYY == YYYY){
        if(mm <= AppMM || MM >= AppMM){
            if(dd <= AppDD || DD >= AppDD){
                return true;
            }
        }
    }
    return false;
}
/*****************************
 * Checks for a specific MM:HH
 * @param mm user input min
 * @param hh user input hour
 * @param MM appointment min
 * @param HH appointment hour
 * @return
 */
bool isValidTime(int mm, int hh, int MM, int HH){
    return !(mm != MM && hh != HH);
}
/****************\
 * Check a valid time range based on the min and max hour input of user
 * @param hh
 * @param HH
 * @param AppHH
 * @return
 */
bool isValidTimeRange(int hh, int HH, int AppHH){
    if(hh <= AppHH && HH >= AppHH)
        return true;
    return false;
}
/*****************************
 * uses the clients appointments stored in a vector to filter based on user inputs
 * @param logapps
 */
void filter_apps(std::vector<appointment> logapps) {
    char* temp;
    int numbytes, count;
    char sendbuf[MAXDATA];
    bool set = false;
    bool t = false;
    bool contains = false;
    bool validate = false;
    std::string mm, dd, yyyy, MM, DD, YYYY, AppMM, AppDD, AppYYYY, AppHH, time1, time2, app, HH, hh;

    while(!set){
        count = 0;
        std::string date1, date2;
        printf("APPOINTMENTS\n");
        for (auto value: logapps) {
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
        while(!contains){
            printf("Enter Start Date-format(MM/DD/YEAR)\n");
            gets(sendbuf);
            numbytes = sizeof(sendbuf);
            sendbuf[numbytes] = '\0';
            std::string str(sendbuf);
            date1 = str;
            contains = check_valid_date(date1);
            //contains = std::regex_match(str, std::regex("^(0[1-9]|1[0-2])\\/(0[1-9]|1\\d|2\\d|3[01])\\/(19|20)\\d{2}$"));
        }
        contains = false;
        while(!contains){
            printf("Enter End Date-format(MM/DD/YEAR)\n");
            gets(sendbuf);
            numbytes = sizeof(sendbuf);
            sendbuf[numbytes] = '\0';
            std::string str2(sendbuf);
            date2 = str2;
            contains = check_valid_date(date2);
            //contains = std::regex_match(str2, std::regex("^(0[1-9]|1[0-2])\\/(0[1-9]|1\\d|2\\d|3[01])\\/(19|20)\\d{2}$"));
        }
        temp = (char*)calloc(128, sizeof(char));
        strncpy(temp, date1.c_str(),128);
        mm = strtok(temp, "/");
        dd = strtok(nullptr, "/");
        yyyy = strtok(nullptr,"");
        temp=(char*)calloc(128, sizeof(char));
        strncpy(temp, date2.c_str(),128);
        MM = strtok(temp, "/");
        DD = strtok(nullptr,"/");
        YYYY = strtok(nullptr,"");
        count = 0;
        for (auto value: logapps) {
            temp = (char *) calloc(512, sizeof(char));
            strncpy(temp, value.get_start_date().c_str(),128);
            AppMM = strtok(temp, "/");
            AppDD = strtok(nullptr, "/");
            AppYYYY = strtok(nullptr,"");
            validate = isValidDate(std::stoi(mm), std::stoi(dd), std::stoi(yyyy), std::stoi(MM), std::stoi(DD),
                                   std::stoi(YYYY),std::stoi(AppMM), std::stoi(AppDD), std::stoi(AppYYYY));
            if(validate){//If it is a valid date them print it out for the user and keep in the logapps
                app = "[" + std::to_string(count) + "]   " + value.get_start_time() + " " +
                      value.get_start_date() + " " +
                      value.get_end_time() + " " + value.get_end_date() + "\n" +
                      value.get_place() + "\n" + value.get_app_details()+"\n";
                strncpy(temp, app.c_str(), 512);
                printf("%s\n", temp);
            }//else{//If it is not a valid date then remove it from the logapps
               // logapps.erase(logapps.begin()+ count);
            //}
            count += 1;
        }
        if(logapps.empty()){
            printf("No Appointments in that range\n");
            break;
        }
        while(!t){
            printf("Filter by specific apointment time[1]\nFilter by appointment time range[2]\nExit Filter[3]\n");
            gets(sendbuf);
            numbytes = sizeof(sendbuf);
            sendbuf[numbytes] = '\0';
            //todo filter by time filter by range exit be done
            int n = atoi(sendbuf);
            switch(n){
                case 1:{
                    contains = false;
                    while(!contains){
                        printf("Enter Start time-format(MM:HH)\n");
                        gets(sendbuf);
                        numbytes = sizeof(sendbuf);
                        sendbuf[numbytes] = '\0';
                        std::string str(sendbuf);
                        time1 = str;
                        contains = check_valid_time(time1);
                        //contains = std::regex_match(str, std::regex("^([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$"));
                    }
                    temp = (char*)calloc(128, sizeof(char));
                    strncpy(temp, time1.c_str(),128);
                    hh = strtok(temp, ":");
                    mm = strtok(nullptr, "");
                    count = 0;
                    for (auto value: logapps) {
                        temp = (char *) calloc(512, sizeof(char));
                        strncpy(temp, value.get_start_time().c_str(),128);
                        AppHH = strtok(temp, ":");
                        AppMM = strtok(nullptr, "");
                        //todo check isValidTime
                        validate = isValidTime(std::stoi(mm), std::stoi(hh), std::stoi(AppMM), std::stoi(AppHH));
                        if(validate){//If it is a valid date them print it out for the user and keep in the logapps
                            app = "[" + std::to_string(count) + "]   " + value.get_start_time() + " " +
                                  value.get_start_date() + " " +
                                  value.get_end_time() + " " + value.get_end_date() + "\n" +
                                  value.get_place() + "\n" + value.get_app_details()+"\n";
                            strncpy(temp, app.c_str(), 512);
                            printf("%s\n", temp);
                        }//else{//If it is not a valid date then remove it from the logapps
                            //logapps.erase(logapps.begin()+ count);
                        //}
                        count += 1;
                    }
                    if(logapps.empty()){
                        printf("No Appointments at that time\n");
                        set = false;
                        break;
                    } else{
                        printf("*******************\n");
                        set = true;
                        t = true;
                        break;
                    }
                }case 2:{
                    contains = false;
                    while(!contains){
                        printf("Enter Start time-format(MM:HH)\n");
                        gets(sendbuf);
                        numbytes = sizeof(sendbuf);
                        sendbuf[numbytes] = '\0';
                        std::string str(sendbuf);
                        time1 = str;
                        contains = check_valid_time(time1);
                        //contains = std::regex_match(str, std::regex("^([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$"));
                    }
                    contains = false;
                    while(!contains){
                        printf("Enter End time-format(MM:HH)\n");
                        gets(sendbuf);
                        numbytes = sizeof(sendbuf);
                        sendbuf[numbytes] = '\0';
                        std::string str(sendbuf);
                        time2 = str;
                        contains = check_valid_time(time2);
                        //contains = std::regex_match(str, std::regex("^([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]$"));
                    }
                    temp = (char*)calloc(128, sizeof(char));
                    strncpy(temp, time1.c_str(),128);
                    hh = strtok(temp, ":");
                    mm = strtok(nullptr,"");
                    temp=(char*)calloc(128, sizeof(char));
                    strncpy(temp, time2.c_str(),128);
                    HH = strtok(temp, ":");
                    mm = strtok(nullptr,"");
                    count = 0;
                    printf("Time Range Results\n");
                    for (auto value: logapps) {
                        temp = (char *) calloc(512, sizeof(char));
                        strncpy(temp, value.get_start_time().c_str(),128);
                        AppHH = strtok(temp, ":");
                        AppMM = strtok(nullptr, "");
                        validate = isValidTimeRange(std::stoi(hh), std::stoi(HH), std::stoi(AppHH));
                        if(validate){//If it is a valid date them print it out for the user and keep in the logapps
                            app = "[" + std::to_string(count) + "]   " + value.get_start_time() + " " +
                                  value.get_start_date() + " " +
                                  value.get_end_time() + " " + value.get_end_date() + "\n" +
                                  value.get_place() + "\n" + value.get_app_details()+"\n";
                            strncpy(temp, app.c_str(), 512);
                            printf("%s\n", temp);
                        }else{//If it is not a valid date then remove it from the logapps
                            logapps.erase(logapps.begin()+ count);
                        }
                        count += 1;
                    }
                    if(logapps.empty()){
                        printf("No Appointments in that time range\n");
                        set = false;
                        break;
                    } else{
                        printf("*******************\n");
                        set = true;
                        t = true;
                        break;
                    }
                }
                case 3:{
                    t = true;
                    set = true;
                    break;
                }
                default:continue;
            }
        }
    };
}

bool string_is_valid(char* input){
    std::string str(input);
    return find_if(str.begin(), str.end(),[](char c) { return !(isalnum(c));}) == str.end();
}
bool password_is_valid(char* input){
    std::string str(input);
    if(str.length() >= 8 && str.length() <= 16){
        return find_if(str.begin(), str.end(),[](char c) { return !(isalnum(c));}) == str.end();
    } else
        return false;
}
bool phone_id_valid(char* input){
    std::string str(input);
    return find_if(str.begin(), str.end(), [](char c) {return !(isdigit(c));}) == str.end();
}
bool check_valid_time(std::string str){
    char* temp;
    std::string mm, hh;
    temp=(char*)calloc(128, sizeof(char));
    strncpy(temp, str.c_str(),128);
    if(isdigit(temp[0]) && isdigit(temp[1]) && temp[2] == ':' && isdigit(temp[3]) && isdigit(temp[4])){
        hh = strtok(temp,":");
        mm= strtok(nullptr,"");
        return std::stoi(hh) >= 0 && std::stoi(hh) <= 24 && std::stoi(mm) >= 0 && std::stoi(mm) <= 59;
    } else
        return false;
}
bool check_valid_date(std::string str){
    char* temp;
    std::string mm, dd, yyyy;
    temp=(char*)calloc(128, sizeof(char));
    strncpy(temp, str.c_str(), 128);
    if(isdigit(temp[0]) && isdigit(temp[1]) && temp[2] == '/' && isdigit(temp[3]) && isdigit(temp[4]) && temp[5] == '/' && isdigit(temp[6]) && isdigit(temp[7]) && isdigit(temp[8]) && isdigit(temp[9])){
        mm = strtok(temp,"/");
        dd = strtok(nullptr,"/");
        yyyy = strtok(nullptr, "");
        return std::stoi(mm) >= 1 && std::stoi(mm) <= 12 && std::stoi(dd) >= 1 && std::stoi(dd) <= 31 && std::stoi(yyyy) >= 1900 && std::stoi(yyyy) <= 2100;
    } else
        return false;
}