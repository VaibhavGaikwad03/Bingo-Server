//
// Created for BingoServer.
//

#ifndef CHAT_MESSAGE_RESPONSE_H
#define CHAT_MESSAGE_RESPONSE_H

#include "IMessageResponse.h"
#include "../message_structures.h"
#include "../utils/utils.h"

class ChatMessageResponse final : public IMessageResponse
{
    ChatMessage _chat_message;

public:
    explicit ChatMessageResponse(const ChatMessage& chat_message);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif //CHAT_MESSAGE_RESPONSE_H
