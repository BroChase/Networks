//
// Created by Chase Brown on 3/22/2018.
//

#ifndef NL2_SERVER_PASSWORDFUNCTIONS_H
#define NL2_SERVER_PASSWORDFUNCTIONS_H

std::string random_string( size_t length );
std::string decrypt(std::string const& msg, std::string const& key);
std::string encrypt(std::string msg, std::string const& key);

#endif //NL2_SERVER_PASSWORDFUNCTIONS_H
