//
// Created by vaibz on 04/10/25.
//

#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/TokenLoginResponse.h"

TokenLoginResponse::TokenLoginResponse(Status status, TokenLoginErrorCode error_code) : _status(status),
    _error_code(error_code)
{
}

nlohmann::json TokenLoginResponse::to_json() const
{
    return {
        {MessageKey::MESSAGE_TYPE, MessageType::RECONNECT_RESPONSE},
        {MessageKey::STATUS, _status},
        {MessageKey::ERROR_CODE, _error_code},
    };
}

Status TokenLoginResponse::get_status() const
{
    return _status;
}

TokenLoginErrorCode TokenLoginResponse::get_error_code() const
{
    return _error_code;
}
