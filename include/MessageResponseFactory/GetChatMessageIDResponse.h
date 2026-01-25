//
// Created by vaibz on 25/01/26.
//

#ifndef GETCHATMESSAGEIDRESPONSE_H
#define GETCHATMESSAGEIDRESPONSE_H
#include "IMessageResponse.h"
#include "../typedefs.h"

class GetChatMessageIDResponse : public IMessageResponse
{
    MessageID _message_id;

public:
    explicit GetChatMessageIDResponse(MessageID message_id);
    [[nodiscard]] nlohmann::json to_json() const override;
    [[nodiscard]] MessageID get_message_id() const;
};

#endif //GETCHATMESSAGEIDRESPONSE_H