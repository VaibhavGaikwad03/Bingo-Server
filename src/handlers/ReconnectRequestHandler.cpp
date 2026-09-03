#include "../../include/handlers/ReconnectRequestHandler.h"
#include "../../include/handlers/HandlerUtils.h"
#include "../../include/message_handler.h"
#include "../../include/user_session_manager.h"
#include "../../include/utils/logger.h"
#include "../../include/debug.h"
#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/ReconnectMessageResponse.h"

void ReconnectRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_reconnect_request(data);

    const std::optional<ReconnectResponse> reconnect_response = _message_handler.reconnect_request(data);
    if (!reconnect_response.has_value())
    {
        const ReconnectResponse reconnect_response_error(Status::ERROR, -1, "", ReconnectErrorCode::SOMETHING_WENT_WRONG);
        const nlohmann::json reconnect_response_error_message = reconnect_response_error.to_json();

        ws->send(reconnect_response_error_message.dump(), uWS::TEXT);
        return;  // CRITICAL: was missing, caused dereference of nullopt below
    }

    const UserID user_id = data[MessageKey::USER_ID].get<UserID>();

    if (reconnect_response->get_status() == Status::SUCCESS)
    {
        UserSessionManager &session_manager = UserSessionManager::instance();

        // Attach this socket to the user. If a session already exists on another
        // socket, it is forcibly logged out and replaced, atomically.
        const LogoutMessageResponse logout_message_response(Status::SUCCESS);
        const std::string kick_payload = logout_message_response.to_json().dump();
        session_manager.attach_session(user_id, _message_handler.get_username(user_id), ws, kick_payload);

        UserSessionManager::instance().display_sessions();

        const nlohmann::json reconnect_response_message = reconnect_response->to_json();

        ws->send(reconnect_response_message.dump(), uWS::TEXT);

        HandlerUtils::send_user_login_payloads(user_id, ws, _message_handler);

        log(Log::INFO, __func__, "Reconnect successful of user id: " + std::to_string(user_id));
    }
    else
    {
        const nlohmann::json reconnect_response_message = reconnect_response->to_json();

        ws->send(reconnect_response_message.dump(), uWS::TEXT);

        log(Log::ERROR, __func__, "Reconnect unsuccessful of user id: " + std::to_string(user_id));
    }
}
