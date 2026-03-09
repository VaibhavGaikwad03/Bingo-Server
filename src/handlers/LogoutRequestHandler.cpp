#include "../../include/handlers/LogoutRequestHandler.h"
#include "../../include/message_handler.h"
#include "../../include/user_session_manager.h"
#include "../../include/utils/logger.h"
#include "../../include/debug.h"
#include "../../include/message_keys.h"

void LogoutRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_logout_request(data);

    // Casting away constness here because nlohmann::json is sometimes required non-const, but we shouldn't modify it in this handler
    nlohmann::json non_const_data = data;
    const std::optional<LogoutMessageResponse> logout_message_response = _message_handler.logout_request(non_const_data);
    if (!logout_message_response.has_value())
    {
        log(Log::ERROR, __func__,
            "Something went wrong while logging out user \'" + std::string(data[MessageKey::USERNAME]) + "\'");

        const LogoutMessageResponse logout_message_error_response(Status::ERROR);
        const nlohmann::json logout_response = logout_message_error_response.to_json();

        log(Log::DEBUG, __func__, logout_response.dump());

        ws->send(logout_response.dump(), uWS::TEXT);

        return;
    }

    if (logout_message_response->get_status() == Status::ERROR)
    {
        log(Log::ERROR, __func__,
            "Failed to log out user \'" + std::string(data[MessageKey::USERNAME]) + "\'");
    }
    else
    {
        UserSessionManager &session_manager = UserSessionManager::instance();
        UserSession *session = session_manager.get_session(data[MessageKey::USER_ID].get<UserID>());
        if (session == nullptr)
        {
            log(Log::INFO, __func__,
            "User \'" + std::string(data[MessageKey::USERNAME]) + "\' failed to logged out. Session not found.");
            return;  // CRITICAL: was missing, caused use-after-null
        }
        session_manager.delete_session(session);

        session_manager.display_sessions();

        log(Log::INFO, __func__,
            "User \'" + std::string(data[MessageKey::USERNAME]) + "\' logged out successfully");
    }

    // const LogoutMessageResponse logout_message_response(status);
    const nlohmann::json logout_response = logout_message_response->to_json();

    log(Log::DEBUG, __func__, logout_response.dump());

    ws->send(logout_response.dump(), uWS::TEXT);
}
