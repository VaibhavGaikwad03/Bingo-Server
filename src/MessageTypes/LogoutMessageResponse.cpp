//
// Created by vaibz on 23/08/25.
//

#include "LogoutMessageResponse.h"
#include "../../include/message_keys.h"

LogoutMessageResponse::LogoutMessageResponse(const Status status) : _status(status)
{
}

nlohmann::json LogoutMessageResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGOUT_RESPONSE},
        {MessageKeys::STATUS, _status}
    };
}
