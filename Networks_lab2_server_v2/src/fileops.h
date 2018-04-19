//
// Created by Chase Brown on 3/21/2018.
//

#ifndef NL2_SERVER_FILEOPS_H
#define NL2_SERVER_FILEOPS_H
#include "users.h"

void save_new_user(user);
bool look_up_users(std::string, user&);

std::vector<appointment> get_user_appointments(std::string name);
void save_appointment(std::string name, appointment newappointment);
void load_appointments(std::vector<appointment> &loguserapp, std::string name);
void save_updated_appointments(std::string name, std::vector<appointment> apps);
#endif //NL2_SERVER_FILEOPS_H
