//
// Created by vaibz on 23/08/25.
//

#ifndef LOGOUTMESSAGERESPONSE_H
#define LOGOUTMESSAGERESPONSE_H
#include "IMessageResponse.h"
#include "../message_types.h"

class LogoutMessageResponse final : public IMessageResponse
{
    Status _status;

public:
    explicit LogoutMessageResponse(Status status);
    [[nodiscard]] nlohmann::json to_json() const override;

    [[nodiscard]] Status get_status() const;
};

#endif // LOGOUTMESSAGERESPONSE_H
