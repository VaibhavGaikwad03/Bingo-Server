#pragma once

#include "IRequestHandler.h"

class LogoutRequestHandler : public IRequestHandler
{
public:
    explicit LogoutRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
