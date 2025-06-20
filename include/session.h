#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "authenticator.h"
#include "../include/uWebSockets/App.h"

struct Session
{
    UserID user_id;
    std::string username;
    uWS::WebSocket<false, uWS::SERVER, std::string> *ws;
};

#endif //SESSION_H
