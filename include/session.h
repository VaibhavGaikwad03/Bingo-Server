#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "message_handler.h"
#include "uWebSockets/App.h"

struct Session
{
    UserID user_id;
    std::string username;
    uWS::WebSocket<false, uWS::SERVER, std::string> *ws; // if we want to give feature of multiple device login. then here we need to maintain list of websockets
    int reference_count;
};

#endif //SESSION_H
