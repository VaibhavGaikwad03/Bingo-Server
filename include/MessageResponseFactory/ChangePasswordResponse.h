//
// Created by vaibz on 26/08/25.
//

#ifndef CHANGEPASSWORDRESPONSE_H
#define CHANGEPASSWORDRESPONSE_H

#include "IMessageResponse.h"
#include "../error_codes.h"
#include "../message_types.h"

class ChangePasswordResponse final : public IMessageResponse
{
    Status _status;
    ChangePasswordErrorCodes _error_code;

public:
    ChangePasswordResponse(Status status, ChangePasswordErrorCodes error_code);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif //CHANGEPASSWORDRESPONSE_H