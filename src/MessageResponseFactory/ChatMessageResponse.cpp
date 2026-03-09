//
// Created for BingoServer.
//

#include "../../include/MessageResponseFactory/ChatMessageResponse.h"

ChatMessageResponse::ChatMessageResponse(const ChatMessage& chat_message)
    : _chat_message(chat_message)
{
}

nlohmann::json ChatMessageResponse::to_json() const
{
    return nlohmann::json{
        {"message_type", utils::to_underlying(_chat_message.message_type)},
        {"message_id", _chat_message.message_id},
        {"conversation_type", utils::to_underlying(_chat_message.conversation_type)},
        {"sender_id", _chat_message.sender_id},
        {"receiver_id", _chat_message.receiver_id},
        {"content_type", utils::to_underlying(_chat_message.content_type)},
        {"content", _chat_message.content},
        {"message_status", utils::to_underlying(_chat_message.message_status)},
        {"is_reply_message", _chat_message.is_reply_message},
        {"replied_message_id", _chat_message.replied_message_id},
        {"timestamp", _chat_message.timestamp}
    };
}
