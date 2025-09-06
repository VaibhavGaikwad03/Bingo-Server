//
// Created by vaibz on 06/09/25.
//

#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/ReconnectMessageResponse.h"

ReconnectResponse::ReconnectResponse(const Status status, const ReconnectErrorCodes error_code) : _status(status),
    _reconnect_error_code(error_code)
{
}


nlohmann::json ReconnectResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::RECONNECT_RESPONSE},
        {MessageKeys::STATUS, _status},
        {MessageKeys::ERROR_CODE, _reconnect_error_code},
    };
}

Status ReconnectResponse::get_status() const
{
    return _status;
}

ReconnectErrorCodes ReconnectResponse::get_reconnect_error_code() const
{
    return _reconnect_error_code;
}