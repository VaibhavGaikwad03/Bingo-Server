//
// Created by vaibz on 23/08/25.
//

#include <utility>
#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/LoginMessageResponse.h"

LoginMessageResponse::LoginMessageResponse(const Status status, const UserID user_id,
                                           const LoginErrorCode error_code, std::string auth_token) : _status(status),
    _user_id(user_id),
    _error_code(error_code),
    _auth_token(std::move(auth_token))
{
}

nlohmann::json LoginMessageResponse::to_json() const
{
    return {
        {MessageKey::MESSAGE_TYPE, MessageType::LOGIN_RESPONSE},
        {MessageKey::STATUS, _status},
        {MessageKey::USER_ID, _user_id},
        {MessageKey::ERROR_CODE, _error_code},
        {MessageKey::AUTH_TOKEN, _auth_token}
    };
}

Status LoginMessageResponse::get_status() const
{
    return _status;
}

UserID LoginMessageResponse::get_userid() const
{
    return _user_id;
}

LoginErrorCode LoginMessageResponse::get_error_code() const
{
    return _error_code;
}

std::string LoginMessageResponse::get_token() const
{
    return _auth_token;
}