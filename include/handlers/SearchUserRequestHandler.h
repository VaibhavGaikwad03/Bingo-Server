#pragma once

#include "IRequestHandler.h"

class SearchUserRequestHandler : public IRequestHandler
{
public:
    explicit SearchUserRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
