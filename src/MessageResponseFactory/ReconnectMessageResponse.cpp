//
// Created by vaibz on 06/09/25.
//

#include <utility>

#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/ReconnectMessageResponse.h"

ReconnectResponse::ReconnectResponse(const Status status, const UserID user_id, std::string auth_token,
                                     const ReconnectErrorCode error_code) : _status(status), _user_id(user_id),
                                                                            _auth_token(std::move(auth_token)),
                                                                            _error_code(error_code)
{
}


nlohmann::json ReconnectResponse::to_json() const
{
    return {
        {MessageKey::MESSAGE_TYPE, MessageType::RECONNECT_RESPONSE},
        {MessageKey::STATUS, _status},
        {MessageKey::USER_ID, _user_id},
        {MessageKey::AUTH_TOKEN, _auth_token},
        {MessageKey::ERROR_CODE, _error_code},
    };
}

Status ReconnectResponse::get_status() const
{
    return _status;
}

ReconnectErrorCode ReconnectResponse::get_error_code() const
{
    return _error_code;
}
