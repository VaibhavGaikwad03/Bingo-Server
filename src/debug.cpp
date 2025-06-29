#include <iostream>
#include "../include/debug.h"

void print_login_request(nlohmann::json message)
{
    std::cout << message["message_type"] << std::endl;
    std::cout << message["username"] << std::endl;
    std::cout << message["password"] << std::endl;
    std::cout << message["timestamp"] << std::endl;
}

void print_signup_request(nlohmann::json message)
{
    std::cout << message["message_type"] << std::endl;
    std::cout << message["username"] << std::endl;
    std::cout << message["password"] << std::endl;
    std::cout << message["fullname"] << std::endl;
    std::cout << message["gender"] << std::endl;
    std::cout << message["dob"] << std::endl;
    std::cout << message["email"] << std::endl;
    std::cout << message["phone"] << std::endl;
    std::cout << message["timestamp"] << std::endl;
}

void print_search_user_request(nlohmann::json message)
{
    std::cout << message["message_type"] << std::endl;
    std::cout << message["username"] << std::endl;
    std::cout << message["requested_by"] << std::endl;
}