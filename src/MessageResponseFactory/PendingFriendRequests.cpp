//
// Created by vaibz on 26/08/25.
//

#include <utility>

#include "../../include/MessageResponseFactory/PendingFriendRequests.h"

#include "../../include/message_keys.h"
#include "../../include/message_types.h"

PendingFriendRequests::PendingFriendRequests(const size_t count, const std::vector<PendingFriendRequestInfo> &pending_friend_requests): _count(count), _pending_friend_requests(pending_friend_requests)
{
}

nlohmann::json PendingFriendRequests::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageType::USER_PENDING_FRIEND_REQUESTS_LIST},
        {MessageKeys::COUNT, _count},
        {MessageKeys::PENDING_FRIEND_REQUESTS_LIST, _pending_friend_requests}
    };
}
