//
// Created by vaibz on 11/6/25.
//

#include "../include/log.h"
#include "../include/utils.h"
#include "../include/error_codes.h"
#include "../include/message_types.h"
#include "../include/nlohmann/json.hpp"
#include "../include/message_processor.h"

#include "../include/debug.h"
#include "../include/session_manager.h"


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

            MessageTypes message_type = parsed_message["message_type"];

            switch (message_type)
            {
                case MessageTypes::LOGIN_REQUEST:
                {
                    // print_login_request(parsed_message); // debug

                    // for now it will send response from current thread
                    UserID result = _authenticator.login(parsed_message["username"], parsed_message["password"]);
                    if (result == utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND))
                    {
                        log(Log::ERROR, "", "Username does not exists: " + std::string(parsed_message["username"]));

                        nlohmann::json login_response = {
                            {"message_type", MessageTypes::LOGIN_RESPONSE},
                            {"status", Status::ERROR},
                            {"user_id", ErrorCodes::INVALID_USER_ID},
                            {"error_code", LoginErrorCodes::USERNAME_NOT_FOUND}
                        };

                        packet.ws->send(login_response.dump(), uWS::TEXT);
                    }
                    else if (result == utils::to_underlying(LoginErrorCodes::PASSWORD_IS_INCORRECT))
                    {
                        log(Log::ERROR, "",
                            "Password is incorrect for user: " + std::string(parsed_message["username"]));

                        nlohmann::json login_response = {
                            {"message_type", MessageTypes::LOGIN_RESPONSE},
                            {"status", Status::ERROR},
                            {"user_id", ErrorCodes::INVALID_USER_ID},
                            {"error_code", LoginErrorCodes::PASSWORD_IS_INCORRECT}
                        };

                        packet.ws->send(login_response.dump(), uWS::TEXT);
                    }
                    else
                    {
                        log(Log::INFO, "",
                            "User '" + std::string(parsed_message["username"]) + "' logged in successfully");

                        nlohmann::json login_response = {
                            {"message_type", MessageTypes::LOGIN_RESPONSE},
                            {"status", Status::SUCCESS},
                            {"user_id", result},
                            {"error_code", LoginErrorCodes::NONE}
                        };

                        SessionManager *session_manager = SessionManager::instance();

                        session_manager->create_session(result, parsed_message["username"], packet.ws);
                        // session created

                        SessionManager::instance()->display_sessions(); // debug purpose

                        packet.ws->send(login_response.dump(), uWS::TEXT);
                    }
                }
                break;

                case MessageTypes::SIGN_UP_REQUEST:
                {
                    // print_signup_request(parsed_message); // debug

                    UserID result = _authenticator.signup(parsed_message["username"], parsed_message["password"],
                                                          parsed_message["fullname"], parsed_message["gender"],
                                                          parsed_message["dob"], parsed_message["email"],
                                                          parsed_message["phone"], parsed_message["timestamp"]);
                    if (result == utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS))
                    {
                        log(Log::ERROR, "", "Username already exists: " + std::string(parsed_message["username"]));

                        nlohmann::json signup_response = {
                            {"message_type", MessageTypes::SIGN_UP_RESPONSE},
                            {"status", Status::ERROR},
                            {"user_id", ErrorCodes::INVALID_USER_ID},
                            {"error_code", SignupErrorCodes::USERNAME_ALREADY_EXISTS}
                        };

                        packet.ws->send(signup_response.dump(), uWS::TEXT);
                    }
                    else if (result == utils::to_underlying(SignupErrorCodes::EMAIL_ALREADY_EXISTS))
                    {
                        log(Log::ERROR, "", "Email already exists: " + std::string(parsed_message["email"]));

                        nlohmann::json signup_response = {
                            {"message_type", MessageTypes::SIGN_UP_RESPONSE},
                            {"status", Status::ERROR},
                            {"user_id", ErrorCodes::INVALID_USER_ID},
                            {"error_code", SignupErrorCodes::EMAIL_ALREADY_EXISTS}
                        };

                        packet.ws->send(signup_response.dump(), uWS::TEXT);
                    }
                    else if (result == utils::to_underlying(SignupErrorCodes::PHONE_ALREADY_EXISTS))
                    {
                        log(Log::ERROR, "", "Phone already exists: " + std::string(parsed_message["phone"]));

                        nlohmann::json signup_response = {
                            {"message_type", MessageTypes::SIGN_UP_RESPONSE},
                            {"status", Status::ERROR},
                            {"user_id", ErrorCodes::INVALID_USER_ID},
                            {"error_code", SignupErrorCodes::PHONE_ALREADY_EXISTS}
                        };

                        packet.ws->send(signup_response.dump(), uWS::TEXT);
                    }
                    else
                    {
                        nlohmann::json signup_response = {
                            {"message_type", MessageTypes::SIGN_UP_RESPONSE},
                            {"status", Status::SUCCESS},
                            {"user_id", result},
                            {"error_code", SignupErrorCodes::NONE}
                        };

                        packet.ws->send(signup_response.dump(), uWS::TEXT);
                    }
                }
                break;

                case MessageTypes::SEARCH_USER_REQUEST:
                {
                    print_search_user_request(parsed_message); // debug
                }
                break;

                default:
                    log(Log::ERROR, "", "Invalid Message Type");
                    break;
            }
        }
        catch (const std::exception &e)
        {
            log(Log::ERROR, "", e.what());
        }
    }
}
