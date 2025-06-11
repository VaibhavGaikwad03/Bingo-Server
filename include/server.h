//
// Created by vaibz on 7/6/25.
//

#ifndef SERVER_H
#define SERVER_H

#include <uWebSockets/App.h>
#include "../include/mutex_queue.h"
#include "../include/message_processor.h"
#include "../include/message_structures.h"

// struct UserData
// {
//     std::string packet;
// };

class Server
{
    uWS::App _app;
    MutexQueue<DataPacket> _mtx_queue;
    MessageProcessor *_message_processor;

    std::condition_variable _cv; // for notifying the message processor thread

    static void connection_opened(uWS::WebSocket<false, uWS::SERVER, std::string> *ws);

    static void connection_closed(uWS::WebSocket<false, uWS::SERVER, std::string> *ws, int code,
                                  std::string_view reason);

    void message_received(uWS::WebSocket<false, uWS::SERVER, std::string> *ws, std::string_view data,
                          uWS::OpCode opCode);

public:
    Server();

    ~Server();

    void run();
};

#endif //SERVER_H
