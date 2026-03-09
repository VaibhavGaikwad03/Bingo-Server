#include "../../include/handlers/UpdateProfileRequestHandler.h"
#include "../../include/message_handler.h"
#include "../../include/utils/logger.h"
#include "../../include/debug.h"
#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/UpdateProfileResponse.h"

void UpdateProfileRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_update_profile_request(data);

    const std::optional<UpdateProfileResponse> update_profile_response = _message_handler.update_profile_request(data);
    if (!update_profile_response.has_value())
    {
        const UpdateProfileResponse update_profile_response_error(Status::ERROR,
                                                                  UpdateProfileErrorCode::SOMETHING_WENT_WRONG);
        const nlohmann::json update_profile_response_error_message = update_profile_response_error.to_json();

        ws->send(update_profile_response_error_message.dump(), uWS::TEXT);
        return;  // CRITICAL: was missing, caused dereference of nullopt below
    }

    if (update_profile_response->get_status() == Status::SUCCESS)
    {
        const nlohmann::json update_profile_response_message = update_profile_response->to_json();

        ws->send(update_profile_response_message.dump(), uWS::TEXT);

        UserID user_id = data[MessageKey::USER_ID].get<UserID>();

        std::optional<UserProfileMessage> user_profile_message = _message_handler.get_user_profile(user_id);
        if (user_profile_message.has_value())
        {
            nlohmann::json user_profile = user_profile_message->to_json();
            log(Log::DEBUG, __func__, user_profile.dump());
            ws->send(user_profile.dump(), uWS::TEXT);
        }

        log(Log::INFO, __func__,
            "Update profile of user '" + data[MessageKey::USERNAME].get<std::string>() + "' is successful");
    }
    else
    {
        log(Log::ERROR, __func__,
            "Update profile of user '" + data[MessageKey::USERNAME].get<std::string>() + "' is unsuccessful");
    }
}
