#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "message_handler.h"
#include "uWebSockets/App.h"

struct Session
{
    UserID user_id;
    std::string username;
    uWS::WebSocket<false, uWS::SERVER, std::string> *ws;
};

#endif //SESSION_H
