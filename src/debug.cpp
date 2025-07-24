#include <iostream>
#include "../include/log.h"
#include "../include/debug.h"
#include "../include/message_keys.h"

static void print_key_value(std::string_view key, const nlohmann::json &message)
{
    std::cout << key << ": " << message[key] << std::endl;
}

void print_login_request(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKeys::MESSAGE_TYPE, message);
        print_key_value(MessageKeys::USERNAME, message);
        print_key_value(MessageKeys::PASSWORD, message);
        print_key_value(MessageKeys::TIMESTAMP, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_logout_request(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKeys::MESSAGE_TYPE, message);
        print_key_value(MessageKeys::USER_ID, message);
        print_key_value(MessageKeys::USERNAME, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_signup_request(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKeys::MESSAGE_TYPE, message);
        print_key_value(MessageKeys::USERNAME, message);
        print_key_value(MessageKeys::PASSWORD, message);
        print_key_value(MessageKeys::FULLNAME, message);
        print_key_value(MessageKeys::GENDER, message);
        print_key_value(MessageKeys::DOB, message);
        print_key_value(MessageKeys::EMAIL, message);
        print_key_value(MessageKeys::PHONE, message);
        print_key_value(MessageKeys::TIMESTAMP, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_search_user_request(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKeys::MESSAGE_TYPE, message);
        print_key_value(MessageKeys::USERNAME, message);
        print_key_value(MessageKeys::REQUESTED_BY, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_friend_req_request(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKeys::MESSAGE_TYPE, message);
        print_key_value(MessageKeys::SENDER_ID, message);
        print_key_value(MessageKeys::SENDER, message);
        print_key_value(MessageKeys::NAME_OF_SENDER, message);
        print_key_value(MessageKeys::RECEIVER_ID, message);
        print_key_value(MessageKeys::RECEIVER, message);
        print_key_value(MessageKeys::NAME_OF_RECEIVER, message);
        print_key_value(MessageKeys::TIMESTAMP, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_friend_req_response(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKeys::MESSAGE_TYPE, message);
        print_key_value(MessageKeys::SENDER_ID, message);
        print_key_value(MessageKeys::SENDER, message);
        print_key_value(MessageKeys::NAME_OF_SENDER, message);
        print_key_value(MessageKeys::RECEIVER_ID, message);
        print_key_value(MessageKeys::RECEIVER, message);
        print_key_value(MessageKeys::NAME_OF_RECEIVER, message);
        print_key_value(MessageKeys::REQUEST_STATUS, message);
        print_key_value(MessageKeys::TIMESTAMP, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}
