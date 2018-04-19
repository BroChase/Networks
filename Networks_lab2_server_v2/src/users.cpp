//
// Created by Chase Brown on 3/20/2018.
//
#include <iostream>
#include <cstring>
//#include <regex>
#include <algorithm>
#include "users.h"

//user functions
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