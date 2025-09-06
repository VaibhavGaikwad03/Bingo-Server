//
// Created by vaibz on 24/08/25.
//

#ifndef SEARCHREQUESTMESSAGERESPONSE_H
#define SEARCHREQUESTMESSAGERESPONSE_H

#include "IMessageResponse.h"

class SearchUserRequestMessageResponse final : public IMessageResponse
{
    int _count;
    nlohmann::json _users;

public:
    SearchUserRequestMessageResponse(int count, nlohmann::json users);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // SEARCHREQUESTMESSAGERESPONSE_H