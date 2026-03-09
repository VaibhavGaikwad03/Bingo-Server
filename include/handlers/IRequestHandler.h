#pragma once

#include "../nlohmann/json.hpp"
#include "../typedefs.h"

// Forward declaration
class MessageHandler;

class IRequestHandler
{
protected:
    MessageHandler& _message_handler;

public:
    explicit IRequestHandler(MessageHandler& message_handler) : _message_handler(message_handler)
    {
    }
    virtual ~IRequestHandler() = default;
    
    // Each handler must implement this execute function
    virtual void handle(WebSocket* ws, const nlohmann::json& data) const = 0;
};
