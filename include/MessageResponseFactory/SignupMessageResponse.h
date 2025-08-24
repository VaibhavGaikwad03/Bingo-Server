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
    SignupErrorCodes _signup_error_codes;

public:
    SignupMessageResponse(Status status, UserID user_id, SignupErrorCodes signup_error_codes);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // SIGNUPMESSAGERESPONSE_H