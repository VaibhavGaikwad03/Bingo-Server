//
// Created by vaibz on 06/09/25.
//

#ifndef RECONNECTMESSAGERESPONSE_H
#define RECONNECTMESSAGERESPONSE_H

#include "IMessageResponse.h"
#include "../error_codes.h"
#include "../message_types.h"
#include "../typedefs.h"

class ReconnectResponse final : public IMessageResponse
{
    Status _status;
    UserID _user_id;
    std::string _auth_token;
    ReconnectErrorCode _error_code;

public:
    explicit ReconnectResponse(Status status, UserID user_id, std::string auth_token, ReconnectErrorCode error_code);
    [[nodiscard]] nlohmann::json to_json() const override;

    [[nodiscard]] Status get_status() const;
    [[nodiscard]] ReconnectErrorCode get_error_code() const;
};

#endif // RECONNECTMESSAGERESPONSE_H