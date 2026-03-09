#pragma once

#include "IRequestHandler.h"

class SignupRequestHandler : public IRequestHandler
{
public:
    explicit SignupRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
