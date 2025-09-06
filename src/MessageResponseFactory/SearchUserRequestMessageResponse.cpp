//
// Created by vaibz on 24/08/25.
//

#include <utility>

#include "../../include/MessageResponseFactory/SearchUserRequestMessageResponse.h"

#include "../../include/message_keys.h"
#include "../../include/message_types.h"

SearchUserRequestMessageResponse::SearchUserRequestMessageResponse(const int count, nlohmann::json users) : _count(count),
    _users(std::move(users))
{
}

nlohmann::json SearchUserRequestMessageResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::SEARCH_USER_RESPONSE},
        {MessageKeys::COUNT, _count},
        {MessageKeys::USERS, _users}
    };
}
