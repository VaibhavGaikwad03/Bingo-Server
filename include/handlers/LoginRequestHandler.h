#pragma once

#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
public:
    explicit LoginRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
