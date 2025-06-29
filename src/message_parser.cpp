#include "../include/message_parser.h"

// MessageParser *MessageParser::_instance = nullptr;

MessageParser::MessageParser()
= default;

MessageParser::~MessageParser()
= default;

// MessageParser *MessageParser::instance()
// {
//     if (_instance == nullptr)
//     {
//         _instance = new MessageParser();
//         return _instance;
//     }
//     return _instance;
// }

// void MessageParser::destroy_instance()
// {
//     if (_instance != nullptr)
//     {
//         delete _instance;
//         _instance = nullptr;
//     }
// }

LoginMessageRequest MessageParser::login_message_request(const nlohmann::json &message)
{
    LoginMessageRequest parsed_message;

    parsed_message.message_type = message["message_type"];
    parsed_message.username = message["username"];
    parsed_message.password = message["password"];
    parsed_message.timestamp = message["timestamp"];

    return parsed_message;
}

SignUpMessageRequest MessageParser::signup_message_request(const nlohmann::json &message)
{
    SignUpMessageRequest parsed_message;

    parsed_message.message_type = message["message_type"];
    parsed_message.username = message["username"];
    parsed_message.password = message["password"];
    parsed_message.fullname = message["fullname"];
    parsed_message.gender = message["gender"];
    parsed_message.dob = message["dob"];
    parsed_message.email = message["email"];
    parsed_message.phone = message["phone"];
    parsed_message.timestamp = message["timestamp"];

    return parsed_message;
}

SearchUserRequest MessageParser::search_user_request(const nlohmann::json &message)
{
    SearchUserRequest parsed_message;

    parsed_message.message_type = message["message_type"];
    parsed_message.username = message["username"];
    parsed_message.requested_by = message["requested_by"];

    return parsed_message;
}