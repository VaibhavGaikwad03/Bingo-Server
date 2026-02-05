//
// Created by vaibz on 25/01/26.
//

#include "../../include/MessageResponseFactory/GetChatMessageIDResponse.h"
#include "../../include/message_keys.h"
#include "../../include/message_types.h"


GetChatMessageIDResponse::GetChatMessageIDResponse(const MessageID message_id) : _message_id(message_id)
{
}

nlohmann::json GetChatMessageIDResponse::to_json() const
{
    return {
            {MessageKey::MESSAGE_TYPE, MessageType::GET_CHAT_MESSAGE_ID_RESPONSE},
            {MessageKey::CHAT_MESSAGE_ID, _message_id}
    };
}

MessageID GetChatMessageIDResponse::get_message_id() const
{
    return _message_id;
}
