//
// Created by vaibz on 26/08/25.
//

#include <utility>

#include "../../include/MessageResponseFactory/PendingFriendRequests.h"

#include "../../include/message_keys.h"
#include "../../include/message_types.h"

PendingFriendRequests::PendingFriendRequests(nlohmann::json request_list) : _request_list(std::move(request_list))
{
}

nlohmann::json PendingFriendRequests::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::USER_PENDING_FRIEND_REQUESTS_LIST},
        {MessageKeys::PENDING_FRIEND_REQUESTS_LIST, _request_list}
    };
}
