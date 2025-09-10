//
// Created by vaibz on 24/08/25.
//

#ifndef SEARCHREQUESTMESSAGERESPONSE_H
#define SEARCHREQUESTMESSAGERESPONSE_H

#include "IMessageResponse.h"
#include "../typedefs.h"

struct FoundUser
{
    UserID user_id;
    std::string username;
    std::string display_name;
    FriendshipState friendship_status;
};

inline void to_json(nlohmann::json &json, const FoundUser &found_user)
{
    json = nlohmann::json{
        {MessageKeys::USER_ID, found_user.user_id},
        {MessageKeys::USERNAME, found_user.username},
        {MessageKeys::DISPLAY_NAME, found_user.display_name},
        {MessageKeys::FRIENDSHIP_STATUS, found_user.friendship_status}
    };
}

class SearchUserRequestMessageResponse final : public IMessageResponse
{
    int _count;
    std::vector<FoundUser> _search_results;

public:
    SearchUserRequestMessageResponse(int count, std::vector<FoundUser> search_results);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // SEARCHREQUESTMESSAGERESPONSE_H
