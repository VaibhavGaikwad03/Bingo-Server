//
// Created by vaibz on 24/08/25.
//

#include <utility>

#include "../../include/MessageResponseFactory/SearchUserRequestResponse.h"

#include "../../include/message_keys.h"
#include "../../include/message_types.h"

SearchUserRequestResponse::SearchUserRequestResponse(const int count, nlohmann::json::array_t users) : _count(count),
    _users(std::move(users))
{
}

nlohmann::json SearchUserRequestResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
        {MessageKeys::COUNT, _count},
        {MessageKeys::USERS, _users}
    };
}
