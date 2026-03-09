//
// Created by vaibz on 09/03/26.
//

#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/ChatHistoryListMessage.h"

ChatHistoryListMessage::ChatHistoryListMessage(size_t count, const std::vector<ChatMessage>& chat_list)
    : _count(count), _chat_list(chat_list)
{
}

nlohmann::json ChatHistoryListMessage::to_json() const
{
    return nlohmann::json{
        {MessageKey::MESSAGE_TYPE, MessageType::USER_MESSAGE_HISTORY},
        {MessageKey::COUNT, _count},
        {MessageKey::CHAT_HISTORY_LIST, _chat_list},
    };
}
