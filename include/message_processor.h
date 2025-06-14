//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <condition_variable>

#include "../include/mutex_queue.h"
#include "../include/authenticator.h"
#include "../include/message_processor.h"
#include "../include/message_structures.h"

class MessageProcessor
{
    std::mutex _mtx;
    std::condition_variable &_cv;
    MutexQueue<DataPacket> &_mtx_queue;

    Authenticator _authenticator;
public:
    explicit MessageProcessor(MutexQueue<DataPacket> &queue, std::condition_variable &cv);
    ~MessageProcessor();

    void process();
};

#endif //MESSAGE_PROCESSOR_H
