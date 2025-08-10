#include "../include/log.h"
#include "../include/message_keys.h"
#include "../include/message_parser.h"

// MessageParser *MessageParser::_instance = nullptr;

MessageParser::MessageParser()
= default;

MessageParser::~MessageParser()
= default;

std::optional<LoginMessageRequest> MessageParser::login_message_request(const nlohmann::json &message)
{
    try
    {
        LoginMessageRequest parsed_message;

        parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
        parsed_message.username = message[MessageKeys::USERNAME];
        parsed_message.password = message[MessageKeys::PASSWORD];
        parsed_message.timestamp = message[MessageKeys::TIMESTAMP];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<LogoutMessageRequest> MessageParser::logout_message_request(const nlohmann::json &message)
{
    try
    {
        LogoutMessageRequest parsed_message;

        parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
        parsed_message.user_id = message[MessageKeys::USER_ID];
        parsed_message.username = message[MessageKeys::USERNAME];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<SignUpMessageRequest> MessageParser::signup_message_request(const nlohmann::json &message)
{
    try
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
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<SearchUserRequest> MessageParser::search_user_request(const nlohmann::json &message)
{
    try
    {
        SearchUserRequest parsed_message;

        parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
        parsed_message.username = message[MessageKeys::USERNAME];
        parsed_message.requested_by = message[MessageKeys::REQUESTED_BY];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<FriendReqRequest> MessageParser::friend_req_request(const nlohmann::json &message)
{
    try
    {
        FriendReqRequest parsed_message;

        parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
        parsed_message.sender_id = message[MessageKeys::SENDER_ID];
        parsed_message.sender = message[MessageKeys::SENDER];
        parsed_message.name_of_sender = message[MessageKeys::NAME_OF_SENDER];
        parsed_message.receiver_id = message[MessageKeys::RECEIVER_ID];
        parsed_message.receiver = message[MessageKeys::RECEIVER];
        parsed_message.name_of_receiver = message[MessageKeys::NAME_OF_RECEIVER];
        parsed_message.timestamp = message[MessageKeys::TIMESTAMP];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<FriendReqResponse> MessageParser::friend_req_response(const nlohmann::json &message)
{
    try
    {
        FriendReqResponse parsed_message;

        parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
        parsed_message.sender_id = message[MessageKeys::SENDER_ID];
        parsed_message.sender = message[MessageKeys::SENDER];
        parsed_message.name_of_sender = message[MessageKeys::NAME_OF_SENDER];
        parsed_message.receiver_id = message[MessageKeys::RECEIVER_ID];
        parsed_message.receiver = message[MessageKeys::RECEIVER];
        parsed_message.name_of_receiver = message[MessageKeys::NAME_OF_RECEIVER];
        parsed_message.status = message[MessageKeys::REQUEST_STATUS];
        parsed_message.timestamp = message[MessageKeys::TIMESTAMP];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<ChangePassword> MessageParser::change_password_request(const nlohmann::json &message)
{
    try
    {
        ChangePassword parsed_message;

        parsed_message.message_type = message[MessageKeys::MESSAGE_TYPE];
        parsed_message.user_id = message[MessageKeys::USER_ID];
        parsed_message.old_password = message[MessageKeys::OLD_PASSWORD];
        parsed_message.new_password = message[MessageKeys::NEW_PASSWORD];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}
