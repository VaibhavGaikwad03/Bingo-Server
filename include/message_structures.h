//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_STRUCTURES_H
#define MESSAGE_STRUCTURES_H

#include <uWebSockets/App.h>

struct DataPacket
{
    uWS::WebSocket<false, uWS::SERVER, std::string> *ws;
    std::string_view data;
    uWS::OpCode opCode;
};

#endif //MESSAGE_STRUCTURES_H
