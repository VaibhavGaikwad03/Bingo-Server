#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include "nlohmann/json.hpp"
#include "message_structures.h"

class MessageParser
{
    MessageParser();
    ~MessageParser();

    // static MessageParser *_instance;

public:
    // static MessageParser *instance();
    // static void destroy_instance();

    MessageParser(const MessageParser&) = delete;
    MessageParser& operator=(const MessageParser&) = delete;

    static LoginMessageRequest login_message_request(const nlohmann::json &message);
    static SignUpMessageRequest signup_message_request(const nlohmann::json &message);
    static SearchUserRequest search_user_request(const nlohmann::json &message);
};

#endif //MESSAGE_PARSER_H
