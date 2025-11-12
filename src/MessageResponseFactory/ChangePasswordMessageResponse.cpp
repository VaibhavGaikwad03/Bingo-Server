//
// Created by vaibz on 26/08/25.
//

#include "../../include/MessageResponseFactory/ChangePasswordMessageResponse.h"

#include "../../include/message_keys.h"

ChangePasswordResponse::ChangePasswordResponse(const Status status, const ChangePasswordErrorCode error_code) : _status(status),
    _error_code(error_code)
{
}

nlohmann::json ChangePasswordResponse::to_json() const
{
    return {
        {MessageKey::MESSAGE_TYPE, MessageType::CHANGE_PASSWORD_RESPONSE},
        {MessageKey::STATUS, Status::ERROR},
        {MessageKey::ERROR_CODE, _error_code}
    };
}
