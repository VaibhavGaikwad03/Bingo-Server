#include "../../include/handlers/ChatMessageHandler.h"
#include "../../include/message_handler.h"
#include "../../include/user_session_manager.h"
#include "../../include/debug.h"
#include "../../include/message_keys.h"

void ChatMessageHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_chat_message(data);

    _message_handler.chat_message(data);

    UserSession *session = UserSessionManager::instance().get_session(data[MessageKey::RECEIVER_ID].get<UserID>());
    if (session != nullptr)
    {
         session->ws->send(data.dump(), uWS::TEXT);
    }
}
