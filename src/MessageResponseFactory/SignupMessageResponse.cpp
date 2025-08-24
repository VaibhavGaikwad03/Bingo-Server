//
// Created by vaibz on 24/08/25.
//

#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/SignupMessageResponse.h"

SignupMessageResponse::SignupMessageResponse(Status status, UserID user_id,
                                             SignupErrorCodes signup_error_codes) : _status(status), _user_id(user_id),
                                                                                    _signup_error_codes(signup_error_codes)
{
}

nlohmann::json SignupMessageResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
        {MessageKeys::STATUS, _status},
        {MessageKeys::USER_ID, _user_id},
        {MessageKeys::ERROR_CODE, _signup_error_codes}
    };
}