//
// Created by vaibz on 06/09/25.
//

#ifndef FRIENDSLISTMESSAGE_H
#define FRIENDSLISTMESSAGE_H

#include "IMessageResponse.h"
#include "../message_keys.h"
#include "../typedefs.h"

struct FriendInfo
{
    UserID user_id;
    std::string username;
    std::string full_name;
    std::string gender;
    std::string dob;
};

inline void to_json(nlohmann::json& json, const FriendInfo& friend_info)
{
    json = nlohmann::json{
        {MessageKeys::USER_ID, friend_info.user_id},
        {MessageKeys::USERNAME, friend_info.username},
        {MessageKeys::FULLNAME, friend_info.full_name},
        {MessageKeys::GENDER, friend_info.gender},
        {MessageKeys::DOB, friend_info.dob},
    };
}

class FriendsListMessage final : public IMessageResponse
{
    size_t _count;
    std::vector<FriendInfo> _friends_list;
public:
    FriendsListMessage(size_t count, const std::vector<FriendInfo>& friends_list);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // FRIENDSLISTMESSAGE_H
