//
// Created by vaibz on 23/08/25.
//

#ifndef LOGOUTMESSAGERESPONSE_H
#define LOGOUTMESSAGERESPONSE_H
#include "IMessageResponse.h"
#include "../../include/message_types.h"

class LogoutMessageResponse final : public IMessageResponse
{
    Status _status;

public:
    explicit LogoutMessageResponse(Status status);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // LOGOUTMESSAGERESPONSE_H
