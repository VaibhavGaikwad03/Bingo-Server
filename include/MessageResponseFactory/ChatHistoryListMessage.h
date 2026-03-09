//
// Created by vaibz on 09/03/26.
//

#ifndef CHATHISTORYLISTMESSAGE_H
#define CHATHISTORYLISTMESSAGE_H

#include "IMessageResponse.h"
#include "../message_keys.h"
#include "../typedefs.h"
#include "../../include/message_structures.h"

inline void to_json(nlohmann::json& json, const ChatMessage& chat_message)
{
    json = nlohmann::json{
        {MessageKey::CHAT_MESSAGE_ID, chat_message.message_id},
        {MessageKey::CONVERSATION_TYPE, chat_message.conversation_type},
        {MessageKey::SENDER_ID, chat_message.sender_id},
        {MessageKey::RECEIVER_ID, chat_message.receiver_id},
        {MessageKey::CONTENT_TYPE, chat_message.content_type},
        {MessageKey::CONTENT, chat_message.content},
        {MessageKey::MESSAGE_STATUS, chat_message.message_status},
        {MessageKey::IS_REPLY_MESSAGE, chat_message.is_reply_message},
        {MessageKey::REPLIED_MESSAGE_ID, chat_message.replied_message_id},
        {MessageKey::TIMESTAMP, chat_message.timestamp},
    };
}

class ChatHistoryListMessage final : public IMessageResponse
{
    size_t _count;
    std::vector<ChatMessage> _chat_list;
public:
    ChatHistoryListMessage(size_t count, const std::vector<ChatMessage>& chat_list);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif //CHATHISTORYLISTMESSAGE_H