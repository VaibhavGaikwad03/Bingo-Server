//
// Created by vaibz on 25/09/25.
//

#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/UpdateProfileResponse.h"

UpdateProfileResponse::UpdateProfileResponse(const Status status,
                                             const UpdateProfileErrorCode error_code) : _status(status),
    _error_code(error_code)
{
}

nlohmann::json UpdateProfileResponse::to_json() const
{
    return {
        {MessageKey::MESSAGE_TYPE, MessageType::UPDATE_PROFILE_RESPONSE},
        {MessageKey::STATUS, _status},
        {MessageKey::ERROR_CODE, _error_code},
    };
}

Status UpdateProfileResponse::get_status() const
{
    return _status;
}

UpdateProfileErrorCode UpdateProfileResponse::get_error_code() const
{
    return _error_code;
}
