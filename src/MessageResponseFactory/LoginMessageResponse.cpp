//
// Created by vaibz on 23/08/25.
//

#include <utility>

#include "../../include/MessageResponseFactory/LoginMessageResponse.h"
#include "../../include/message_keys.h"

LoginMessageResponse::LoginMessageResponse(const Status status, const UserID user_id,
                                           const LoginErrorCodes login_error_codes, std::string auth_token) : _status(status),
    _user_id(user_id),
    _login_error_codes(login_error_codes),
    _auth_token(std::move(auth_token))
{
}

nlohmann::json LoginMessageResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE},
        {MessageKeys::STATUS, _status},
        {MessageKeys::USER_ID, _user_id},
        {MessageKeys::ERROR_CODE, _login_error_codes},
        {MessageKeys::AUTH_TOKEN, _auth_token}
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

LoginErrorCodes LoginMessageResponse::get_login_error_codes() const
{
    return _login_error_codes;
}

std::string LoginMessageResponse::get_token() const
{
    return _auth_token;
}