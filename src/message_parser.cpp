#include "../include/message_keys.h"
#include "../include/message_parser.h"

// MessageParser *MessageParser::_instance = nullptr;

MessageParser::MessageParser()
= default;

MessageParser::~MessageParser()
= default;

LoginMessageRequest MessageParser::login_message_request(const nlohmann::json &message)
{
    LoginMessageRequest parsed_message;

    parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
    parsed_message.username = message[MessageKeys::USERNAME];
    parsed_message.password = message[MessageKeys::PASSWORD];
    parsed_message.timestamp = message[MessageKeys::TIMESTAMP];

    return parsed_message;
}

SignUpMessageRequest MessageParser::signup_message_request(const nlohmann::json &message)
{
    SignUpMessageRequest parsed_message;

    parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
    parsed_message.username = message[MessageKeys::USERNAME];
    parsed_message.password = message[MessageKeys::PASSWORD];
    parsed_message.fullname = message[MessageKeys::FULLNAME];
    parsed_message.gender = message[MessageKeys::GENDER];
    parsed_message.dob = message[MessageKeys::DOB];
    parsed_message.email = message[MessageKeys::EMAIL];
    parsed_message.phone = message[MessageKeys::PHONE];
    parsed_message.timestamp = message[MessageKeys::TIMESTAMP];

    return parsed_message;
}

SearchUserRequest MessageParser::search_user_request(const nlohmann::json &message)
{
    SearchUserRequest parsed_message;

    parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
    parsed_message.username = message[MessageKeys::USERNAME];
    parsed_message.requested_by = message[MessageKeys::REQUESTED_BY];

    return parsed_message;
}

FriendReqRequest MessageParser::friend_req_request(const nlohmann::json &message)
{
    FriendReqRequest parsed_message;

    parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
    parsed_message.sender_id = message[MessageKeys::SENDER_ID];
    parsed_message.sender = message[MessageKeys::SENDER];
    parsed_message.receiver_id = message[MessageKeys::RECEIVER_ID];
    parsed_message.receiver = message[MessageKeys::RECEIVER];
    parsed_message.timestamp = message[MessageKeys::TIMESTAMP];

    return parsed_message;
}
