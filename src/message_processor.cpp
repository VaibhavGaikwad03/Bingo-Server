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
#include "../include/message_keys.h"
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

            switch (MessageTypes message_type = packet_data[MessageKeys::MESSAGE_TYPE])
            {
                case MessageTypes::LOGIN_REQUEST:
                {
                    // print_login_request(parsed_message); // debug

                    // for now it will send response from current thread
                    UserID result = _message_handler.login(packet_data);
                    if (result == utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND))
                    {
                        log(Log::ERROR, "",
                            "Username does not exists: " + std::string(packet_data[MessageKeys::USERNAME]));

                        nlohmann::json login_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE},
                            {MessageKeys::STATUS, Status::ERROR},
                            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
                            {MessageKeys::ERROR_CODE, LoginErrorCodes::USERNAME_NOT_FOUND}
                        };

                        packet.ws->send(login_response.dump(), uWS::TEXT);
                    }
                    else if (result == utils::to_underlying(LoginErrorCodes::PASSWORD_IS_INCORRECT))
                    {
                        log(Log::ERROR, "",
                            "Password is incorrect for user: " + std::string(packet_data[MessageKeys::USERNAME]));

                        nlohmann::json login_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE},
                            {MessageKeys::STATUS, Status::ERROR},
                            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
                            {MessageKeys::ERROR_CODE, LoginErrorCodes::PASSWORD_IS_INCORRECT}
                        };

                        packet.ws->send(login_response.dump(), uWS::TEXT);
                    }
                    else
                    {
                        log(Log::INFO, "",
                            "User '" + std::string(packet_data[MessageKeys::USERNAME]) + "' logged in successfully");

                        nlohmann::json login_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE},
                            {MessageKeys::STATUS, Status::SUCCESS},
                            {MessageKeys::USER_ID, result},
                            {MessageKeys::ERROR_CODE, LoginErrorCodes::NONE}
                        };

                        SessionManager *session_manager = SessionManager::instance();

                        session_manager->create_session(result, packet_data[MessageKeys::USERNAME], packet.ws);
                        // session created

                        SessionManager::instance()->display_sessions(); // debug purpose

                        packet.ws->send(login_response.dump(), uWS::TEXT);

                        send_user_login_payloads(result, packet.ws);
                    }
                }
                break;

                case MessageTypes::SIGN_UP_REQUEST:
                {
                    // print_signup_request(parsed_message); // debug

                    UserID result = _message_handler.signup(packet_data);
                    if (result == utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS))
                    {
                        log(Log::ERROR, "",
                            "Username already exists: " + std::string(packet_data[MessageKeys::USERNAME]));

                        nlohmann::json signup_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
                            {MessageKeys::STATUS, Status::ERROR},
                            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
                            {MessageKeys::ERROR_CODE, SignupErrorCodes::USERNAME_ALREADY_EXISTS}
                        };

                        packet.ws->send(signup_response.dump(), uWS::TEXT);
                    }
                    else if (result == utils::to_underlying(SignupErrorCodes::EMAIL_ALREADY_EXISTS))
                    {
                        log(Log::ERROR, "", "Email already exists: " + std::string(packet_data[MessageKeys::EMAIL]));

                        nlohmann::json signup_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
                            {MessageKeys::STATUS, Status::ERROR},
                            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
                            {MessageKeys::ERROR_CODE, SignupErrorCodes::EMAIL_ALREADY_EXISTS}
                        };

                        packet.ws->send(signup_response.dump(), uWS::TEXT);
                    }
                    else if (result == utils::to_underlying(SignupErrorCodes::PHONE_ALREADY_EXISTS))
                    {
                        log(Log::ERROR, "", "Phone already exists: " + std::string(packet_data["phone"]));

                        nlohmann::json signup_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
                            {MessageKeys::STATUS, Status::ERROR},
                            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
                            {MessageKeys::ERROR_CODE, SignupErrorCodes::PHONE_ALREADY_EXISTS}
                        };

                        packet.ws->send(signup_response.dump(), uWS::TEXT);
                    }
                    else
                    {
                        nlohmann::json signup_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
                            {MessageKeys::STATUS, Status::SUCCESS},
                            {MessageKeys::USER_ID, result},
                            {MessageKeys::ERROR_CODE, SignupErrorCodes::NONE}
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
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::SEARCH_USER_RESPONSE},
                            {MessageKeys::COUNT, 0},
                            {MessageKeys::USERS, nlohmann::json::array()}
                        };

                        packet.ws->send(search_user_response.dump(), uWS::TEXT);
                    }
                    else
                    {
                        nlohmann::json search_user_response = {
                            {MessageKeys::MESSAGE_TYPE, MessageTypes::SEARCH_USER_RESPONSE},
                            {MessageKeys::COUNT, found_users.size()},
                            {MessageKeys::USERS, nlohmann::json::array()}
                        };

                        for (const auto &found_user: found_users)
                        {
                            search_user_response[MessageKeys::USERS].push_back({
                                {MessageKeys::USER_ID, found_user.user_id},
                                {MessageKeys::USERNAME, found_user.username},
                                {MessageKeys::DISPLAY_NAME, found_user.name},
                                {MessageKeys::FRIENDSHIP_STATUS, found_user.friendship_status}
                            });
                        }

                        // std::cout << search_user_response.dump() << std::endl;

                        std::cout << "sent: " << search_user_response.dump() << std::endl;
                        packet.ws->send(search_user_response.dump(), uWS::TEXT);
                    }
                }
                break;

                case MessageTypes::FRIEND_REQ_REQUEST:
                {
                    print_friend_req_request(packet_data);

                    _message_handler.friend_req_request(packet_data);

                    try
                    {
                        Session *session = SessionManager::instance()->get_session(
                            std::atoi(packet_data[MessageKeys::RECEIVER_ID].dump().c_str()));
                        if (session) // session found
                        {
                            std::cout << packet_data.dump() << std::endl;
                            session->ws->send(packet_data.dump(), uWS::TEXT);
                        }
                    }
                    catch (std::exception &e)
                    {
                        log(Log::ERROR, "", e.what());
                    }
                }
                break;

                case MessageTypes::FRIEND_REQ_RESPONSE:
                {
                    print_friend_req_response(packet_data);
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

void MessageProcessor::send_user_login_payloads(const UserID user_id,
                                                uWS::WebSocket<false, uWS::SERVER, std::string> *ws) const
{
    try
    {
        // fetch user profile
        auto user_profile = _message_handler.get_user_profile(user_id);
        if (!user_profile)
        {
            log(Log::ERROR, __func__, "User profile not found");
            return;
        }


        const nlohmann::json user_profile_message = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::USER_PROFILE_INFORMATION},
            {MessageKeys::FULLNAME, user_profile->name},
            {MessageKeys::USERNAME, user_profile->username},
            {MessageKeys::DOB, user_profile->dob},
            {MessageKeys::GENDER, user_profile->gender},
            {MessageKeys::EMAIL, user_profile->email},
            {MessageKeys::PHONE, user_profile->phone},
        };
        std::cout << user_profile_message.dump() << std::endl;

        ws->send(user_profile_message.dump(), uWS::TEXT);

        // fetch pending friend request list
        std::vector<PendingFriendRequest> pending_friend_requests = _message_handler.
                get_pending_friend_requests(user_id);
        if (!pending_friend_requests.empty())
        {
            nlohmann::json request_list = nlohmann::json::array();
            for (const auto &pending_friend_request: pending_friend_requests)
            {
                request_list.push_back({
                    {MessageKeys::SENDER_ID, pending_friend_request.sender_id},
                    {MessageKeys::SENDER, pending_friend_request.sender},
                    {MessageKeys::NAME_OF_SENDER, pending_friend_request.name_of_sender},
                    {MessageKeys::RECEIVER_ID, pending_friend_request.receiver_id},
                    {MessageKeys::RECEIVER, pending_friend_request.receiver},
                    {MessageKeys::NAME_OF_RECEIVER, pending_friend_request.name_of_receiver},
                    {MessageKeys::REQUEST_STATUS, pending_friend_request.request_status},
                    {MessageKeys::TIMESTAMP, pending_friend_request.timestamp}
                });
                std::cout << request_list.dump() << std::endl;
            }

            nlohmann::json pending_friend_requests_list = {
                {MessageKeys::MESSAGE_TYPE, MessageTypes::USER_PENDING_FRIEND_REQUESTS_LIST},
                {MessageKeys::PENDING_FRIEND_REQUESTS_LIST, request_list}
            };

            std::cout << pending_friend_requests_list.dump() << std::endl;

            ws->send(pending_friend_requests_list.dump(), uWS::TEXT);
        }
    }
    catch (nlohmann::detail::exception &ex)
    {
        log(Log::ERROR, "", ex.what());
    }
}
