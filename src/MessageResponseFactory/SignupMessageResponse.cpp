//
// Created by vaibz on 24/08/25.
//

#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/SignupMessageResponse.h"

SignupMessageResponse::SignupMessageResponse(const Status status, const UserID user_id,
                                             const SignupErrorCode error_code) : _status(status), _user_id(user_id),
                                                                                    _error_codes(error_code)
{
}

nlohmann::json SignupMessageResponse::to_json() const
{
    return {
        {MessageKey::MESSAGE_TYPE, MessageType::SIGN_UP_RESPONSE},
        {MessageKey::STATUS, _status},
        {MessageKey::USER_ID, _user_id},
        {MessageKey::ERROR_CODE, _error_codes}
    };
}

Status SignupMessageResponse::get_status() const
{
    return _status;
}

UserID SignupMessageResponse::get_userid() const
{
    return _user_id;
}

SignupErrorCode SignupMessageResponse::get_error_code() const
{
    return _error_codes;
}