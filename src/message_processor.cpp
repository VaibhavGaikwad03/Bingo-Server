//
// Created by vaibz on 11/6/25.
//

#include "../include/log.h"
#include "../include/utils.h"
#include "../include/error_codes.h"
#include "../include/message_types.h"
#include "../include/nlohmann/json.hpp"
#include "../include/message_processor.h"


MessageProcessor::MessageProcessor(MutexQueue<DataPacket> &queue, std::condition_variable &cv) : _cv(cv),
                                                                                                 _mtx_queue(queue)
{
}

MessageProcessor::~MessageProcessor()
= default;

void MessageProcessor::process()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]
        {
            return !_mtx_queue.empty();
        });

        const DataPacket packet = _mtx_queue.dequeue();

        try
        {
            nlohmann::json parsed_message = nlohmann::json::parse(packet.data);

            if (parsed_message["message_type"] == utils::to_underlying(MessageTypes::LOGIN_REQUEST))
            {
                std::cout << parsed_message["message_type"] << std::endl;
                std::cout << parsed_message["username"] << std::endl;
                std::cout << parsed_message["password"] << std::endl;
                std::cout << parsed_message["timestamp"] << std::endl;
                // packet.ws->send(packet.data, uWS::TEXT);
                // for now it will send response from current thread

                UserID result =  _authenticator.login(parsed_message["username"], parsed_message["password"]);
                if (result == utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND))
                {
                    nlohmann::json login_response;
                    login_response["message_type"] = MessageTypes::LOGIN_RESPONSE;
                    login_response["status"] = Status::ERROR;
                    login_response["user_id"] = -1;
                    login_response["error_code"] = LoginErrorCodes::USERNAME_NOT_FOUND;

                    packet.ws->send(login_response.dump(), uWS::TEXT);
                }
                else if (result == utils::to_underlying(LoginErrorCodes::PASSWORD_IS_INCORRECT))
                {
                    nlohmann::json login_response;
                    login_response["message_type"] = MessageTypes::LOGIN_RESPONSE;
                    login_response["status"] = Status::ERROR;
                    login_response["user_id"] = -1;
                    login_response["error_code"] = LoginErrorCodes::PASSWORD_IS_INCORRECT;

                    packet.ws->send(login_response.dump(), uWS::TEXT);
                }
                else
                {
                    nlohmann::json login_response;
                    login_response["message_type"] = MessageTypes::LOGIN_RESPONSE;
                    login_response["status"] = Status::SUCCESS;
                    login_response["user_id"] = result;
                    login_response["error_code"] = LoginErrorCodes::NONE;

                    packet.ws->send(login_response.dump(), uWS::TEXT);
                }
            }
            else if (parsed_message["message_type"] == utils::to_underlying(MessageTypes::SIGN_UP_REQUEST))
            {
                std::cout << parsed_message["message_type"] << std::endl;
                std::cout << parsed_message["username"] << std::endl;
                std::cout << parsed_message["password"] << std::endl;
                std::cout << parsed_message["fullname"] << std::endl;
                std::cout << parsed_message["gender"] << std::endl;
                std::cout << parsed_message["dob"] << std::endl;
                std::cout << parsed_message["email"] << std::endl;
                std::cout << parsed_message["phone"] << std::endl;
                std::cout << parsed_message["signup_timestamp"] << std::endl;

                UserID result =  _authenticator.signup(parsed_message["username"], parsed_message["password"], parsed_message["fullname"], parsed_message["gender"], parsed_message["dob"], parsed_message["email"], parsed_message["phone"], parsed_message["signup_timestamp"]);
                if (result == utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS))
                {
                    log(Log::ERROR, "", "Username already exists: " + std::string(parsed_message["username"]));
                    nlohmann::json signup_response;
                    signup_response["message_type"] = MessageTypes::SIGN_UP_RESPONSE;
                    signup_response["status"] = Status::ERROR;
                    signup_response["user_id"] = -1;
                    signup_response["error_code"] = SignupErrorCodes::USERNAME_ALREADY_EXISTS;

                    packet.ws->send(signup_response.dump(), uWS::TEXT);
                }
                else if (result == utils::to_underlying(SignupErrorCodes::EMAIL_ALREADY_EXISTS))
                {
                    nlohmann::json signup_response;
                    signup_response["message_type"] = MessageTypes::SIGN_UP_RESPONSE;
                    signup_response["status"] = Status::ERROR;
                    signup_response["user_id"] = -1;
                    signup_response["error_code"] = SignupErrorCodes::EMAIL_ALREADY_EXISTS;

                    packet.ws->send(signup_response.dump(), uWS::TEXT);
                }
                else if (result == utils::to_underlying(SignupErrorCodes::PHONE_ALREADY_EXISTS))
                {
                    nlohmann::json signup_response;
                    signup_response["message_type"] = MessageTypes::SIGN_UP_RESPONSE;
                    signup_response["status"] = Status::ERROR;
                    signup_response["user_id"] = -1;
                    signup_response["error_code"] = SignupErrorCodes::PHONE_ALREADY_EXISTS;

                    packet.ws->send(signup_response.dump(), uWS::TEXT);
                }
                else
                {
                    nlohmann::json signup_response;
                    signup_response["message_type"] = MessageTypes::SIGN_UP_RESPONSE;
                    signup_response["status"] = Status::SUCCESS;
                    signup_response["user_id"] = result;
                    signup_response["error_code"] = SignupErrorCodes::NONE;

                    packet.ws->send(signup_response.dump(), uWS::TEXT);
                }
            }
        }
        catch (const std::exception &e)
        {
            log(Log::ERROR, "", e.what());
        }
    }
}
