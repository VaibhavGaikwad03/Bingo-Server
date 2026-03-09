//
// Created by vaibz on 11/6/25.
//

#include "../include/utils/logger.h"
#include "../include/error_codes.h"
#include "../include/message_types.h"
#include "../include/nlohmann/json.hpp"
#include "../include/message_processor.h"

#include "../include/debug.h"
#include "../include/message_keys.h"
#include "../include/user_session_manager.h"

// Include all specific handlers
#include "../include/handlers/LoginRequestHandler.h"
#include "../include/handlers/LogoutRequestHandler.h"
#include "../include/handlers/SignupRequestHandler.h"
#include "../include/handlers/SearchUserRequestHandler.h"
#include "../include/handlers/FriendReqRequestHandler.h"
#include "../include/handlers/FriendReqResponseHandler.h"
#include "../include/handlers/ChangePasswordRequestHandler.h"
#include "../include/handlers/ReconnectRequestHandler.h"
#include "../include/handlers/UpdateProfileRequestHandler.h"
#include "../include/handlers/ChatMessageHandler.h"
#include "../include/handlers/GetMessageIdRequestHandler.h"

MessageProcessor::MessageProcessor(ThreadSafeQueue<DataPacket> &queue, std::condition_variable &cv, const DatabaseConfig& db_config) 
    : _cv(cv),
    _mtx_queue(queue),
    _message_handler(db_config)
{
    // Register all message handlers
    _handlers[MessageType::LOGIN_REQUEST] = std::make_unique<LoginRequestHandler>(_message_handler);
    _handlers[MessageType::LOGOUT_REQUEST] = std::make_unique<LogoutRequestHandler>(_message_handler);
    _handlers[MessageType::SIGN_UP_REQUEST] = std::make_unique<SignupRequestHandler>(_message_handler);
    _handlers[MessageType::SEARCH_USER_REQUEST] = std::make_unique<SearchUserRequestHandler>(_message_handler);
    _handlers[MessageType::FRIEND_REQ_REQUEST] = std::make_unique<FriendReqRequestHandler>(_message_handler);
    _handlers[MessageType::FRIEND_REQ_RESPONSE] = std::make_unique<FriendReqResponseHandler>(_message_handler);
    _handlers[MessageType::CHANGE_PASSWORD_REQUEST] = std::make_unique<ChangePasswordRequestHandler>(_message_handler);
    _handlers[MessageType::RECONNECT_REQUEST] = std::make_unique<ReconnectRequestHandler>(_message_handler);
    _handlers[MessageType::UPDATE_PROFILE_REQUEST] = std::make_unique<UpdateProfileRequestHandler>(_message_handler);
    _handlers[MessageType::CHAT_MESSAGE] = std::make_unique<ChatMessageHandler>(_message_handler);
    _handlers[MessageType::GET_CHAT_MESSAGE_ID_REQUEST] = std::make_unique<GetMessageIdRequestHandler>(_message_handler);
}

MessageProcessor::~MessageProcessor()
= default;

void MessageProcessor::process()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]
        {
            return !_mtx_queue.empty();
        });

        const DataPacket packet = _mtx_queue.dequeue();

        try
        {
            nlohmann::json packet_data = nlohmann::json::parse(packet.data);

            log(Log::DEBUG, __func__, std::string("in process func: ") + packet_data.dump());

            auto message_type = static_cast<MessageType>(packet_data[MessageKey::MESSAGE_TYPE].get<int>());

            auto handler = _handlers.find(message_type);
            if (handler != _handlers.end())
            {
                handler->second->handle(packet.ws, packet_data);
            }
            else
            {
                log(Log::ERROR, __func__, "Invalid/Unknown Message Type: " + std::to_string(static_cast<int>(message_type)));
            }
        }
        catch (const std::exception &e)
        {
            log(Log::ERROR, __func__, e.what());
        }
    }
}