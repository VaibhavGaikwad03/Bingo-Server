//
// Created by vaibz on 24/08/25.
//

#ifndef SEARCHREQUESTRESPONSE_H
#define SEARCHREQUESTRESPONSE_H

#include "IMessageResponse.h"

class SearchUserRequestResponse final : public IMessageResponse
{
    int _count;
    nlohmann::json _users;

public:
    SearchUserRequestResponse(int count, nlohmann::json users);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // SEARCHREQUESTRESPONSE_H