#pragma once

#include "IRequestHandler.h"

class FriendReqResponseHandler : public IRequestHandler
{
public:
    explicit FriendReqResponseHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
