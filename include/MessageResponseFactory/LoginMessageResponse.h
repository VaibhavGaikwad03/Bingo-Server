//
// Created by vaibz on 23/08/25.
//

#ifndef LOGINMESSAGERESPONSE_H
#define LOGINMESSAGERESPONSE_H
#include "IMessageResponse.h"
#include "../error_codes.h"
#include "../message_types.h"
#include "../typedefs.h"

class LoginMessageResponse final : public IMessageResponse
{
    Status _status;
    UserID _user_id;
    LoginErrorCode _error_code;
    std::string _auth_token;

public:
    LoginMessageResponse(Status status, UserID user_id, LoginErrorCode error_code, std::string auth_token);
    [[nodiscard]] nlohmann::json to_json() const override;

    [[nodiscard]] Status get_status() const;
    [[nodiscard]] UserID get_userid() const;
    [[nodiscard]] LoginErrorCode get_error_code() const;
    [[nodiscard]] std::string get_token() const;
};

#endif // LOGINMESSAGERESPONSE_H
