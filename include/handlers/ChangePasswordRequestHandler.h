#pragma once

#include "IRequestHandler.h"

class ChangePasswordRequestHandler : public IRequestHandler
{
public:
    explicit ChangePasswordRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
