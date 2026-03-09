#pragma once

#include "IRequestHandler.h"

class ReconnectRequestHandler : public IRequestHandler
{
public:
    explicit ReconnectRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
