//
// Created by vaibz on 24/08/25.
//

#include <utility>

#include "../../include/message_keys.h"
#include "../../include/message_types.h"
#include "../../include/MessageResponseFactory/SearchUserRequestMessageResponse.h"

SearchUserRequestMessageResponse::SearchUserRequestMessageResponse(const int count,
                                                                   std::vector<FoundUser>
                                                                   search_results) : _count(count),
    _search_results(std::move(search_results))
{
}

nlohmann::json SearchUserRequestMessageResponse::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageType::SEARCH_USER_RESPONSE},
        {MessageKeys::COUNT, _count},
        {MessageKeys::USERS, _search_results}
    };
}
