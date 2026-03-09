#include "../../include/handlers/LoginRequestHandler.h"
#include "../../include/handlers/HandlerUtils.h"
#include "../../include/message_handler.h"
#include "../../include/user_session_manager.h"
#include "../../include/utils/logger.h"
#include "../../include/message_keys.h"

void LoginRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    const std::optional<LoginMessageResponse> login_message_response = _message_handler.login_request(data);
    if (!login_message_response.has_value())
    {
        log(Log::ERROR, __func__,
            "Something went wrong while logging in: " + std::string(data[MessageKey::USERNAME]));

        const LoginMessageResponse login_message_error_response(Status::ERROR,
                                                          static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                          LoginErrorCode::SOMETHING_WENT_WRONG, "");
        const nlohmann::json login_response = login_message_error_response.to_json();

        ws->send(login_response.dump(), uWS::TEXT);

        return;
    }

    if (login_message_response->get_error_code() == LoginErrorCode::USERNAME_NOT_FOUND)
    {
        log(Log::ERROR, __func__,
            "Username does not exists: " + std::string(data[MessageKey::USERNAME]));
    }
    else if (login_message_response->get_error_code() == LoginErrorCode::PASSWORD_IS_INCORRECT)
    {
        log(Log::ERROR, __func__,
            "Password is incorrect for user: " + std::string(data[MessageKey::USERNAME]));
    }
    else
    {
        UserSessionManager &session_manager = UserSessionManager::instance();
        if (session_manager.is_session_exists(login_message_response->get_userid())) // if session already exists
        {
            UserSession *session = session_manager.get_session(login_message_response->get_userid());

            const LogoutMessageResponse logout_message_response(Status::SUCCESS);
            const nlohmann::json logout_response = logout_message_response.to_json();

            log(Log::INFO, __func__,
                "User \'" + std::string(data[MessageKey::USERNAME]) + "\' logged out forcefully");
            session->ws->send(logout_response.dump(), uWS::TEXT);
            session->ws = ws;
        }
        else
        {
            // new session created
            session_manager.create_session(login_message_response->get_userid(), data[MessageKey::USERNAME], ws);
        }

        UserSessionManager::instance().display_sessions(); // debug purpose

        log(Log::INFO, __func__,
            "User '" + std::string(data[MessageKey::USERNAME]) + "' logged in successfully");
    }

    const nlohmann::json login_response = login_message_response->to_json();
    ws->send(login_response.dump(), uWS::TEXT); // sending login response

    if (login_message_response->get_status() == Status::SUCCESS) // if login success, send login payload
    {
        HandlerUtils::send_user_login_payloads(login_message_response->get_userid(), ws, _message_handler);
    }
}
