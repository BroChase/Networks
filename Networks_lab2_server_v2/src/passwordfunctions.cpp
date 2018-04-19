//
// Created by Chase Brown on 3/22/2018.
//
#include <cstdlib>
#include <cstdio>
#include <string>
#include <random>
#include <algorithm>
#include "passwordfunctions.h"


std::string random_string( size_t length ) {
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

std::string encrypt(std::string msg, std::string const& key)
{
    if(key.empty())
        return msg;
    for (std::string::size_type i = 0; i < msg.size(); ++i)
        msg[i] ^= key[i];
    return msg;
}

std::string decrypt(std::string const& msg, std::string const& key)
{
    return encrypt(msg, key);
}