//
// Created by vaibz on 19/11/25.
//

#ifndef FORCEDLOGOUTMESSAGE_H
#define FORCEDLOGOUTMESSAGE_H

#include "IMessageResponse.h"

class ForcedLogout final : public IMessageResponse
{
public:
    ForcedLogout();
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif // FORCEDLOGOUTMESSAGE_H