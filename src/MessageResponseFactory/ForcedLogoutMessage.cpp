//
// Created by vaibz on 19/11/25.
//

#include "../../include/MessageResponseFactory/ForcedLogoutMessage.h"
#include "../../include/message_keys.h"
#include "../../include/message_types.h"

ForcedLogout::ForcedLogout()
= default;

nlohmann::json ForcedLogout::to_json() const
{
    return
    {
        {MessageKey::MESSAGE_TYPE, MessageType::FORCED_LOGOUT}
    };
}

