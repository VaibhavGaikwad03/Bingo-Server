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
            nlohmann::json packet_data = nlohmann::json::parse(packet.data);

            switch (MessageTypes message_type = packet_data["message_type"])
            {
                case MessageTypes::LOGIN_REQUEST:
                {
                    // print_login_request(parsed_message); // debug

                    // for now it will send response from current thread
                    UserID result = _message_handler.login(packet_data);
                    if (result == utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND))
                    {
                        log(Log::ERROR, "", "Username does not exists: " + std::string(packet_data["username"]));

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
                            "Password is incorrect for user: " + std::string(packet_data["username"]));

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
                            "User '" + std::string(packet_data["username"]) + "' logged in successfully");

                        nlohmann::json login_response = {
                            {"message_type", MessageTypes::LOGIN_RESPONSE},
                            {"status", Status::SUCCESS},
                            {"user_id", result},
                            {"error_code", LoginErrorCodes::NONE}
                        };

                        SessionManager *session_manager = SessionManager::instance();

                        session_manager->create_session(result, packet_data["username"], packet.ws);
                        // session created

                        SessionManager::instance()->display_sessions(); // debug purpose

                        packet.ws->send(login_response.dump(), uWS::TEXT);
                    }
                }
                break;

                case MessageTypes::SIGN_UP_REQUEST:
                {
                    // print_signup_request(parsed_message); // debug

                    UserID result = _message_handler.signup(packet_data);
                    if (result == utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS))
                    {
                        log(Log::ERROR, "", "Username already exists: " + std::string(packet_data["username"]));

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
                        log(Log::ERROR, "", "Email already exists: " + std::string(packet_data["email"]));

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
                        log(Log::ERROR, "", "Phone already exists: " + std::string(packet_data["phone"]));

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
                    //print_search_user_request(packet_data); // debug

                    std::vector<FoundUser> found_users = _message_handler.search_user(packet_data);
                    if (found_users.empty())
                    {
                        nlohmann::json search_user_response = {
                            {"message_type", MessageTypes::SEARCH_USER_RESPONSE},
                            {"count", 0},
                            {"users", nlohmann::json::array()}
                        };

                        packet.ws->send(search_user_response.dump(), uWS::TEXT);
                    }
                    else
                    {
                        nlohmann::json search_user_response = {
                            {"message_type", MessageTypes::SEARCH_USER_RESPONSE},
                            {"count", found_users.size()},
                            {"users", nlohmann::json::array()}
                        };

                        for (const auto &found_user: found_users)
                        {
                            search_user_response["users"].push_back({
                                {"user_id", found_user.user_id},
                                {"username", found_user.username},
                                {"display_name", found_user.name},
                                {"is_friend", FriendshipStatus::NOT_FRIEND}
                            });
                        }

                        std::cout << search_user_response.dump() << std::endl;

                        packet.ws->send(search_user_response.dump(), uWS::TEXT);
                    }
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
