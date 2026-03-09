//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <condition_variable>

#include "thread_safe_queue.h"
#include "message_handler.h"
#include "message_processor.h"
#include "message_structures.h"
#include "utils/config_reader.h"
#include <unordered_map>
#include <memory>
#include "handlers/IRequestHandler.h"

class MessageProcessor
{
    std::mutex _mtx;
    std::condition_variable &_cv;
    ThreadSafeQueue<DataPacket> &_mtx_queue;

    MessageHandler _message_handler;
    std::unordered_map<MessageType, std::unique_ptr<IRequestHandler>> _handlers;

public:
    explicit MessageProcessor(ThreadSafeQueue<DataPacket> &queue, std::condition_variable &cv, const DatabaseConfig& db_config);

    ~MessageProcessor();

    void process();
};

#endif //MESSAGE_PROCESSOR_H
