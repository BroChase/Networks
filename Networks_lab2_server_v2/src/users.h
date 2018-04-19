//
// Created by Chase Brown on 3/20/2018.
//


#ifndef NL2_SERVER_USERS_H
#define NL2_SERVER_USERS_H

#include <string>

class appointment{
private:
    std::string appPlace;
    std::string startTime;
    std::string startDate;
    std::string endTime;
    std::string endDate;
    std::string appDetails;
public:
    appointment() = default;
    //setters
    bool setPlace(std::string);
    bool set_start_time(std::string);
    bool set_start_date(std::string);
    bool set_end_time(std::string);
    bool set_end_date(std::string);
    bool set_app_details(std::string);
    //getters
    std::string get_place();
    std::string get_start_time();
    std::string get_start_date();
    std::string get_end_time();
    std::string get_end_date();
    std::string get_app_details();
};

class user{
private:
    std::string name;
    std::string password;
    std::string phone;
    std::string email;
    std::vector<appointment*> appointments;
public:
    user() = default;
    //setters
    void add_appointment(std::string p, std::string s_t, std::string e_t, std::string s_d,
                         std::string e_d, std::string s_a_d);
    bool setName(char*);
    bool setPassword(char*);
    bool setPhone(char*);
    bool setEmail(char*);
    //getters
    std::string getName();
    std::string getPassword();
    std::string getPhone();
    std::string getEmail();
};


bool string_is_valid(char* str);
bool password_is_valid(char* input);
bool phone_id_valid(char* input);
bool check_valid_time(std::string str);
bool check_valid_date(std::string str);


#endif //NL2_SERVER_USERS_H
