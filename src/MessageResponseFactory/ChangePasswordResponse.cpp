//
// Created by vaibz on 26/08/25.
//

#include "../../include/MessageResponseFactory/ChangePasswordResponse.h"

#include "../../include/message_keys.h"

ChangePasswordResponse::ChangePasswordResponse(const Status status, const ChangePasswordErrorCodes error_code) : _status(status),
    _error_code(error_code)
{
}

nlohmann::json ChangePasswordResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::CHANGE_PASSWORD_RESPONSE},
        {MessageKeys::STATUS, Status::ERROR},
        {MessageKeys::ERROR_CODE, _error_code}
    };
}
