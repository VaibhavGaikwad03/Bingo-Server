//
// Created by vaibz on 04/10/25.
//

#ifndef TOKENLOGINRESPONSE_H
#define TOKENLOGINRESPONSE_H

#include "IMessageResponse.h"
#include "../error_codes.h"
#include "../message_types.h"

class TokenLoginResponse final : public IMessageResponse
{
    Status _status;
    TokenLoginErrorCode _error_code;

public:
    explicit TokenLoginResponse(Status status, TokenLoginErrorCode error_code);
    [[nodiscard]] nlohmann::json to_json() const override;

    [[nodiscard]] Status get_status() const;
    [[nodiscard]] TokenLoginErrorCode get_error_code() const;
};

#endif // TOKENLOGINRESPONSE_H