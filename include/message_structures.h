//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_STRUCTURES_H
#define MESSAGE_STRUCTURES_H

#include "../include/uWebSockets/App.h"

enum class UserCredentialsTableIndex : uint8_t
{
    USER_ID = 0,
    USERNAME = 1,
    PASSWORD = 2,
};

struct DataPacket
{
    uWS::WebSocket<false, uWS::SERVER, std::string> *ws;
    std::string_view data;
    uWS::OpCode opCode;
};

struct Session
{
    uWS::WebSocket<false, uWS::SERVER, std::string> *ws;
};

#endif //MESSAGE_STRUCTURES_H
