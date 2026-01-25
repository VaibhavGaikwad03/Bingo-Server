//
// Created by vaibz on 27/7/25.
//

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "uWebSockets/WebSocket.h"

typedef unsigned int UserID;
typedef uWS::WebSocket<false, uWS::SERVER, std::string> WebSocket;
typedef unsigned int MessageID;

#endif //TYPEDEFS_H
