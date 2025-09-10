//
// Created by vaibz on 24/08/25.
//

#ifndef SIGNUPMESSAGERESPONSE_H
#define SIGNUPMESSAGERESPONSE_H

#include "IMessageResponse.h"
#include "../error_codes.h"
#include "../message_types.h"
#include "../typedefs.h"

class SignupMessageResponse final : public IMessageResponse
{
    Status _status;
    UserID _user_id;
    SignupErrorCode _signup_error_codes;

public:
    SignupMessageResponse(Status status, UserID user_id, SignupErrorCode signup_error_codes);
    [[nodiscard]] nlohmann::json to_json() const override;
    [[nodiscard]] Status get_status() const;
    [[nodiscard]] UserID get_userid() const;
    [[nodiscard]] SignupErrorCode get_signup_error_code() const;
};

#endif // SIGNUPMESSAGERESPONSE_H