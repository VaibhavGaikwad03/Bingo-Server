//
// Created by vaibz on 23/08/25.
//

#include "../../include/MessageTypes/LoginMessageResponse.h"
#include "../../include/message_keys.h"

LoginMessageResponse::LoginMessageResponse(const Status status, const UserID user_id,
                                           const LoginErrorCodes login_error_codes) : _status(status), _user_id(user_id),
    _login_error_codes(login_error_codes)
{
}

nlohmann::json LoginMessageResponse::to_json() const
{
    return nlohmann::json{
        MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE,
        MessageKeys::STATUS, _status,
        MessageKeys::USER_ID, _user_id,
        MessageKeys::ERROR_CODE, _login_error_codes
    };
}
