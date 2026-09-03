//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <thread>

#include "thread_safe_queue.h"
#include "message_handler.h"
#include "message_structures.h"
#include "utils/config_reader.h"
#include <unordered_map>
#include <memory>
#include "handlers/IRequestHandler.h"

// A single worker in the processing pool.
//
// Each MessageProcessor owns its own inbound queue, its own MessageHandler (and
// therefore its own MySQL connection), and its own worker thread. Running N of
// these gives both a thread pool and a DB connection pool of size N. The server
// pins each socket to one worker (see Server::message_received) so that all
// messages from a given connection are processed in order by a single thread.
class MessageProcessor
{
    ThreadSafeQueue<DataPacket> _queue;

    MessageHandler _message_handler;
    std::unordered_map<MessageType, std::unique_ptr<IRequestHandler>> _handlers;

    std::thread _worker;

    void process();

public:
    explicit MessageProcessor(const DatabaseConfig& db_config);

    ~MessageProcessor();

    MessageProcessor(const MessageProcessor&) = delete;
    MessageProcessor& operator=(const MessageProcessor&) = delete;

    // Spawns the worker thread. Call once after construction.
    void start();

    // Hands a packet to this worker's queue.
    void enqueue(const DataPacket& packet);
};

#endif //MESSAGE_PROCESSOR_H
