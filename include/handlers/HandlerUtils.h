#pragma once

#include "../nlohmann/json.hpp"
#include "../typedefs.h"
#include "../message_handler.h"
#include "../utils/logger.h"

// Forward declaration
class MessageProcessor;

namespace HandlerUtils
{
    // Shared utility function that was previously part of MessageProcessor.
    // It's used by both LoginRequestHandler and ReconnectRequestHandler.
    void send_user_login_payloads(const UserID user_id, WebSocket* ws, const MessageHandler& message_handler);
}
