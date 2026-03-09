#include "../../include/handlers/SignupRequestHandler.h"
#include "../../include/utils/logger.h"
#include "../../include/message_handler.h"
#include "../../include/message_keys.h"

void SignupRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    nlohmann::json non_const_data = data;
    const std::optional<SignupMessageResponse> signup_message_response = _message_handler.signup_request(non_const_data);
    
    if (!signup_message_response.has_value())
    {
        log(Log::ERROR, __func__,
            "Something went wrong: " + std::string(data[MessageKey::USERNAME]));

        const SignupMessageResponse signup_message_error_response(Status::ERROR,
                                                                  static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                                  SignupErrorCode::SOMETHING_WENT_WRONG);
        nlohmann::json signup_response = signup_message_error_response.to_json();

        log(Log::DEBUG, __func__, signup_response.dump());

        ws->send(signup_response.dump(), uWS::TEXT);
    }
    else if (signup_message_response->get_error_code() == SignupErrorCode::USERNAME_ALREADY_EXISTS)
    {
        log(Log::ERROR, __func__,
            "Username already exists: " + std::string(data[MessageKey::USERNAME]));
    }
    else if (signup_message_response->get_error_code() == SignupErrorCode::EMAIL_ALREADY_EXISTS)
    {
        log(Log::ERROR, __func__, "Email already exists: " + std::string(data[MessageKey::EMAIL]));
    }
    else if (signup_message_response->get_error_code() == SignupErrorCode::PHONE_ALREADY_EXISTS)
    {
        log(Log::ERROR, __func__, "Phone already exists: " + std::string(data[MessageKey::PHONE]));
    }
    else
    {
        log(Log::INFO, __func__, "Signed up successfully: " + std::string(data[MessageKey::USERNAME]));
    }

    const nlohmann::json signup_response = signup_message_response->to_json();

    ws->send(signup_response.dump(), uWS::TEXT);
}
