#include "../../include/handlers/FriendReqRequestHandler.h"
#include "../../include/message_handler.h"
#include "../../include/user_session_manager.h"
#include "../../include/utils/logger.h"
#include "../../include/debug.h"
#include "../../include/message_keys.h"

void FriendReqRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    // Casting away constness here because nlohmann::json is sometimes required non-const
    nlohmann::json non_const_data = data;
    print_friend_req_request(non_const_data);

    _message_handler.friend_req_request(non_const_data);

    try
    {
        const UserSession *session = UserSessionManager::instance().get_session(
            data[MessageKey::RECEIVER_ID].get<UserID>());
        if (session) // session found
        {
            session->ws->send(data.dump(), uWS::TEXT);
        }
    }
    catch (std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}
