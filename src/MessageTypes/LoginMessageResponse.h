//
// Created by vaibz on 23/08/25.
//

#ifndef LOGINMESSAGERESPONSE_H
#define LOGINMESSAGERESPONSE_H
#include "IMessageResponse.h"
#include "../../include/error_codes.h"
#include "../../include/message_types.h"
#include "../../include/typedefs.h"

class LoginMessageResponse final : public IMessageResponse
{
    Status _status;
    UserID _user_id;
    LoginErrorCodes _login_error_codes;

public:
    LoginMessageResponse(Status status, UserID user_id, LoginErrorCodes login_error_codes);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // LOGINMESSAGERESPONSE_H
