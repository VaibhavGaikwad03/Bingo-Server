//
// Created by vaibz on 23/08/25.
//

#include "../../include/MessageResponseFactory/LogoutMessageResponse.h"
#include "../../include/message_keys.h"

LogoutMessageResponse::LogoutMessageResponse(const Status status) : _status(status)
{
}

nlohmann::json LogoutMessageResponse::to_json() const
{
    return {
        {MessageKey::MESSAGE_TYPE, MessageType::LOGOUT_RESPONSE},
        {MessageKey::STATUS, _status}
    };
}

Status LogoutMessageResponse::get_status() const
{
    return _status;
}
