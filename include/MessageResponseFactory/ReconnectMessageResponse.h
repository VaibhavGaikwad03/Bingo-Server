//
// Created by vaibz on 06/09/25.
//

#ifndef RECONNECTMESSAGERESPONSE_H
#define RECONNECTMESSAGERESPONSE_H

#include "IMessageResponse.h"
#include "../error_codes.h"
#include "../message_types.h"

class ReconnectResponse final : public IMessageResponse
{
    Status _status;
    ReconnectErrorCode _reconnect_error_code;

public:
    explicit ReconnectResponse(Status status, ReconnectErrorCode error_code);
    [[nodiscard]] nlohmann::json to_json() const override;

    Status get_status() const;
    ReconnectErrorCode get_reconnect_error_code() const;
};

#endif // RECONNECTMESSAGERESPONSE_H