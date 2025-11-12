#include <iostream>
#include "../include/utils/logger.h"
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
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::USERNAME, message);
        print_key_value(MessageKey::PASSWORD, message);
        print_key_value(MessageKey::TIMESTAMP, message);
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
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::USER_ID, message);
        print_key_value(MessageKey::USERNAME, message);
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
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::USERNAME, message);
        print_key_value(MessageKey::PASSWORD, message);
        print_key_value(MessageKey::FULLNAME, message);
        print_key_value(MessageKey::GENDER, message);
        print_key_value(MessageKey::DOB, message);
        print_key_value(MessageKey::EMAIL, message);
        print_key_value(MessageKey::PHONE, message);
        print_key_value(MessageKey::TIMESTAMP, message);
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
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::USERNAME, message);
        print_key_value(MessageKey::REQUESTED_BY, message);
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
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::SENDER_ID, message);
        print_key_value(MessageKey::SENDER, message);
        print_key_value(MessageKey::NAME_OF_SENDER, message);
        print_key_value(MessageKey::RECEIVER_ID, message);
        print_key_value(MessageKey::RECEIVER, message);
        print_key_value(MessageKey::NAME_OF_RECEIVER, message);
        print_key_value(MessageKey::TIMESTAMP, message);
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
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::SENDER_ID, message);
        print_key_value(MessageKey::SENDER, message);
        print_key_value(MessageKey::NAME_OF_SENDER, message);
        print_key_value(MessageKey::RECEIVER_ID, message);
        print_key_value(MessageKey::RECEIVER, message);
        print_key_value(MessageKey::NAME_OF_RECEIVER, message);
        print_key_value(MessageKey::REQUEST_STATUS, message);
        print_key_value(MessageKey::TIMESTAMP, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_change_password_request(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::USER_ID, message);
        print_key_value(MessageKey::OLD_PASSWORD, message);
        print_key_value(MessageKey::NEW_PASSWORD, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_reconnect_request(const nlohmann::json& message)
{
    try
    {
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::USER_ID, message);
        print_key_value(MessageKey::AUTH_TOKEN, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void print_update_profile_request(const nlohmann::json &message)
{
    try
    {
        print_key_value(MessageKey::MESSAGE_TYPE, message);
        print_key_value(MessageKey::USER_ID, message);
        print_key_value(MessageKey::USERNAME, message);
        print_key_value(MessageKey::FULLNAME, message);
        print_key_value(MessageKey::DOB, message);
        print_key_value(MessageKey::GENDER, message);
        print_key_value(MessageKey::EMAIL, message);
        print_key_value(MessageKey::PHONE, message);
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}