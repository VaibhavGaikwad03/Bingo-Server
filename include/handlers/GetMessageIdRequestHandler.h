#pragma once

#include "IRequestHandler.h"

class GetMessageIdRequestHandler : public IRequestHandler
{
public:
    explicit GetMessageIdRequestHandler(MessageHandler& message_handler) : IRequestHandler(message_handler)
    {
    }

    void handle(WebSocket* ws, const nlohmann::json& data) const override;
};
