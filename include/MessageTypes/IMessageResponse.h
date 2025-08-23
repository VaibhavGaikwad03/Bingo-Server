//
// Created by vaibz on 23/08/25.
//

#ifndef IMESSAGERESPONSE_H
#define IMESSAGERESPONSE_H
#include "../nlohmann/json.hpp"


class IMessageResponse
{
public:
    virtual ~IMessageResponse() = default;

private:
    [[nodiscard]] virtual nlohmann::json to_json() const = 0;
};

#endif // IMESSAGERESPONSE_H