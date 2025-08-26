//
// Created by vaibz on 26/08/25.
//

#ifndef  PENDINGFRIENDREQUESTS_H
#define  PENDINGFRIENDREQUESTS_H
#include "IMessageResponse.h"

class PendingFriendRequests final : public IMessageResponse
{
    nlohmann::json _request_list;

public:
    explicit PendingFriendRequests(nlohmann::json request_list);
    [[nodiscard]]nlohmann::json to_json() const override;
};

#endif // PENDINGFRIENDREQUESTS_H