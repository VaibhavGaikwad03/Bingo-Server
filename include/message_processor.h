//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <condition_variable>

#include "mutex_queue.h"
#include "message_handler.h"
#include "message_processor.h"
#include "message_structures.h"

class MessageProcessor
{
    std::mutex _mtx;
    std::condition_variable &_cv;
    MutexQueue<DataPacket> &_mtx_queue;

    MessageHandler _message_handler;

    void send_user_login_payloads(uWS::WebSocket<false, uWS::SERVER, std::string> *ws);

public:
    explicit MessageProcessor(MutexQueue<DataPacket> &queue, std::condition_variable &cv);

    ~MessageProcessor();

    void process();
};

#endif //MESSAGE_PROCESSOR_H
