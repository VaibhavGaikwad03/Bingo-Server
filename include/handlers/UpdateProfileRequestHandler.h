#pragma once

#include "IRequestHandler.h"

class UpdateProfileRequestHandler : public IRequestHandler
{
public:
    explicit UpdateProfileRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
