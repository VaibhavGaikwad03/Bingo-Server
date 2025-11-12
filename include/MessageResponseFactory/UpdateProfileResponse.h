//
// Created by vaibz on 25/09/25.
//

#ifndef UPDATEPROFILERESPONSE_H
#define UPDATEPROFILERESPONSE_H

#include "IMessageResponse.h"
#include "../error_codes.h"
#include "../message_types.h"

class UpdateProfileResponse final : public IMessageResponse
{
    Status _status;
    UpdateProfileErrorCode _error_code;

public:
    UpdateProfileResponse(Status status, UpdateProfileErrorCode error_code);
    [[nodiscard]] nlohmann::json to_json() const override;
    [[nodiscard]] Status get_status() const;
    [[nodiscard]] UpdateProfileErrorCode get_error_code() const;
};

#endif // UPDATEPROFILERESPONSE_H
