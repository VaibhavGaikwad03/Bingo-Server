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

    void process_login_request(WebSocket *ws, nlohmann::json &data) const;
    void process_logout_request(WebSocket *ws, nlohmann::json &data) const;
    void process_signup_request(WebSocket *ws, nlohmann::json &data) const;
    void process_search_user_request(WebSocket *ws, nlohmann::json &data) const;
    void process_friend_req_request(WebSocket *ws, nlohmann::json &data) const;
    void process_friend_req_response(WebSocket *ws, const nlohmann::json &data) const;
    void send_user_login_payloads(UserID user_id, WebSocket *ws) const;
    void process_change_password_request(WebSocket *ws, const nlohmann::json &data) const;

public:
    explicit MessageProcessor(MutexQueue<DataPacket> &queue, std::condition_variable &cv);

    ~MessageProcessor();

    void process();
};

#endif //MESSAGE_PROCESSOR_H
