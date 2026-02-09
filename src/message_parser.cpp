#include "../include/utils/logger.h"
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

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.username = message[MessageKey::USERNAME];
        parsed_message.password = message[MessageKey::PASSWORD];
        parsed_message.timestamp = message[MessageKey::TIMESTAMP];

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

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.user_id = message[MessageKey::USER_ID];
        parsed_message.username = message[MessageKey::USERNAME];

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

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.username = message[MessageKey::USERNAME];
        parsed_message.password = message[MessageKey::PASSWORD];
        parsed_message.fullname = message[MessageKey::FULLNAME];
        parsed_message.gender = message[MessageKey::GENDER];
        parsed_message.dob = message[MessageKey::DOB];
        parsed_message.email = message[MessageKey::EMAIL];
        parsed_message.phone = message[MessageKey::PHONE];
        parsed_message.timestamp = message[MessageKey::TIMESTAMP];

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

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.username = message[MessageKey::USERNAME];
        parsed_message.requested_by = message[MessageKey::REQUESTED_BY];

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

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.sender_id = message[MessageKey::SENDER_ID];
        parsed_message.sender = message[MessageKey::SENDER];
        parsed_message.name_of_sender = message[MessageKey::NAME_OF_SENDER];
        parsed_message.receiver_id = message[MessageKey::RECEIVER_ID];
        parsed_message.receiver = message[MessageKey::RECEIVER];
        parsed_message.name_of_receiver = message[MessageKey::NAME_OF_RECEIVER];
        parsed_message.timestamp = message[MessageKey::TIMESTAMP];

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

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.sender_id = message[MessageKey::SENDER_ID];
        parsed_message.sender = message[MessageKey::SENDER];
        parsed_message.name_of_sender = message[MessageKey::NAME_OF_SENDER];
        parsed_message.receiver_id = message[MessageKey::RECEIVER_ID];
        parsed_message.receiver = message[MessageKey::RECEIVER];
        parsed_message.name_of_receiver = message[MessageKey::NAME_OF_RECEIVER];
        parsed_message.status = message[MessageKey::REQUEST_STATUS];
        parsed_message.timestamp = message[MessageKey::TIMESTAMP];

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

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.user_id = message[MessageKey::USER_ID];
        parsed_message.old_password = message[MessageKey::OLD_PASSWORD];
        parsed_message.new_password = message[MessageKey::NEW_PASSWORD];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<ReconnectRequest> MessageParser::reconnect_request(const nlohmann::json &message)
{
    try
    {
        ReconnectRequest parsed_message;

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.user_id = message[MessageKey::USER_ID];
        parsed_message.auth_token = message[MessageKey::AUTH_TOKEN];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<UpdateProfileRequest> MessageParser::update_profile_request(const nlohmann::json &message)
{
    try
    {
        UpdateProfileRequest parsed_message;

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.user_id = message[MessageKey::USER_ID];
        parsed_message.username = message[MessageKey::USERNAME];
        parsed_message.fullname = message[MessageKey::FULLNAME];
        parsed_message.dob = message[MessageKey::DOB];
        parsed_message.gender = message[MessageKey::GENDER];
        parsed_message.email = message[MessageKey::EMAIL];
        parsed_message.phone = message[MessageKey::PHONE];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<GetChatMessageIDRequest> MessageParser::get_chat_message_id_request(const nlohmann::json &message)
{
    try
    {
        GetChatMessageIDRequest parsed_message;

        parsed_message.message_type = message[MessageKey::MESSAGE_TYPE];
        parsed_message.user_id = message[MessageKey::USER_ID];

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}

std::optional<ChatMessage> MessageParser::chat_message(const nlohmann::json &message)
{
    try
    {
        ChatMessage parsed_message;

        parsed_message.message_type = static_cast<MessageType>(message[MessageKey::MESSAGE_TYPE].get<int>());
        parsed_message.message_id = message[MessageKey::CHAT_MESSAGE_ID].get<MessageID>();
        parsed_message.conversation_type = static_cast<ConversationType>(message[MessageKey::CONVERSATION_TYPE].get<int>());
        parsed_message.sender_id = message[MessageKey::SENDER_ID].get<UserID>();
        parsed_message.receiver_id = message[MessageKey::RECEIVER_ID].get<UserID>();
        parsed_message.content_type = static_cast<ContentType>(message[MessageKey::CONTENT_TYPE].get<int>());
        parsed_message.content = message[MessageKey::CONTENT].get<std::string>();
        parsed_message.message_status = static_cast<MessageStatus>(message[MessageKey::MESSAGE_STATUS].get<int>());
        parsed_message.is_reply_message = message[MessageKey::IS_REPLY_MESSAGE].get<int>();
        parsed_message.replied_message_id = message[MessageKey::REPLIED_MESSAGE_ID].get<MessageID>();
        parsed_message.timestamp = message[MessageKey::TIMESTAMP].get<std::string>();

        return parsed_message;
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return std::nullopt;
    }
}
