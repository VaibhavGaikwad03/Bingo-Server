#include "../../include/handlers/ChangePasswordRequestHandler.h"
#include "../../include/message_handler.h"
#include "../../include/utils/logger.h"
#include "../../include/debug.h"
#include "../../include/MessageResponseFactory/ChangePasswordMessageResponse.h"

void ChangePasswordRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_change_password_request(data);

    const ChangePasswordErrorCode result = _message_handler.change_password_request(data);
    if (result == ChangePasswordErrorCode::SOMETHING_WENT_WRONG)
    {
        const ChangePasswordResponse change_password_response(Status::ERROR, result);
        const nlohmann::json change_password = change_password_response.to_json();

        log(Log::ERROR, __func__, "Something went wrong while changing the password");

        ws->send(change_password.dump(), uWS::TEXT);
    }
    else if (result == ChangePasswordErrorCode::NEW_PASSWORD_MUST_BE_DIFFERENT)
    {
        const ChangePasswordResponse change_password_response(Status::ERROR, result);
        const nlohmann::json change_password = change_password_response.to_json();

        log(Log::ERROR, __func__, "New password must be different that old password");

        ws->send(change_password.dump(), uWS::TEXT);
    }
    else
    {
        const ChangePasswordResponse change_password_response(Status::SUCCESS, result);
        const nlohmann::json change_password = change_password_response.to_json();

        log(Log::INFO, __func__, "Password changed successfully");

        ws->send(change_password.dump(), uWS::TEXT);
    }
}
