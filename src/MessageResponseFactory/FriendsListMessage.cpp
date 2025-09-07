//
// Created by vaibz on 06/09/25.
//

#include "../../include/message_types.h"
#include "../../include/message_keys.h"
#include "../../include/MessageResponseFactory/FriendsListMessage.h"

FriendsListMessage::FriendsListMessage(size_t count, const std::vector<FriendInfo> &friends_list) : _count(count),
    _friends_list(friends_list)
{
}

nlohmann::json FriendsListMessage::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageType::USER_FRIENDS_LIST},
        {MessageKeys::COUNT, _count},
        {MessageKeys::FRIENDS_LIST, _friends_list},
    };
}
