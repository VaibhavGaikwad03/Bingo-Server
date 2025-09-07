//
// Created by vaibz on 26/08/25.
//

#ifndef  PENDINGFRIENDREQUESTS_H
#define  PENDINGFRIENDREQUESTS_H
#include "IMessageResponse.h"
#include "../message_keys.h"

struct PendingFriendRequestInfo
{
    int sender_id;
    std::string sender;
    std::string name_of_sender;
    int receiver_id;
    std::string receiver;
    std::string name_of_receiver;
    std::string request_status;
    std::string timestamp;
};

inline void to_json(nlohmann::json &json, const PendingFriendRequestInfo &pending_friend_request_info)
{
    json = nlohmann::json{
        {MessageKeys::SENDER_ID, pending_friend_request_info.sender_id},
        {MessageKeys::SENDER, pending_friend_request_info.sender},
        {MessageKeys::NAME_OF_SENDER, pending_friend_request_info.name_of_sender},
        {MessageKeys::RECEIVER_ID, pending_friend_request_info.receiver_id},
        {MessageKeys::RECEIVER, pending_friend_request_info.receiver},
        {MessageKeys::NAME_OF_RECEIVER, pending_friend_request_info.name_of_receiver},
        {MessageKeys::REQUEST_STATUS, pending_friend_request_info.request_status},
        {MessageKeys::TIMESTAMP, pending_friend_request_info.timestamp},
    };
}

class PendingFriendRequests final : public IMessageResponse
{
    size_t _count;
    std::vector<PendingFriendRequestInfo> _pending_friend_requests;

public:
    explicit PendingFriendRequests(size_t count, const std::vector<PendingFriendRequestInfo> &pending_friend_requests);

    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // PENDINGFRIENDREQUESTS_H
