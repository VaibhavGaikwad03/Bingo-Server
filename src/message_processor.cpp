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
#include "MessageTypes/LoginMessageResponse.h"
#include "MessageTypes/LogoutMessageResponse.h"


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
                    process_login_request(packet.ws, packet_data);
                }
                break;

                case MessageTypes::LOGOUT_REQUEST:
                {
                    process_logout_request(packet.ws, packet_data);
                }
                break;

                case MessageTypes::SIGN_UP_REQUEST:
                {
                    process_signup_request(packet.ws, packet_data);
                }
                break;

                case MessageTypes::SEARCH_USER_REQUEST:
                {
                    process_search_user_request(packet.ws, packet_data);
                }
                break;

                case MessageTypes::FRIEND_REQ_REQUEST:
                {
                    process_friend_req_request(packet.ws, packet_data);
                }
                break;

                case MessageTypes::FRIEND_REQ_RESPONSE:
                {
                    process_friend_req_response(packet.ws, packet_data);
                }
                break;

                case MessageTypes::CHANGE_PASSWORD_REQUEST:
                {
                    process_change_password_request(packet.ws, packet_data);
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
                                                WebSocket *ws) const
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

void MessageProcessor::process_login_request(WebSocket *ws,
                                             nlohmann::json &data) const
{
    // print_login_request(parsed_message); // debug

    // for now, it will send response from current thread
    UserID result = _message_handler.login(data);
    if (result == utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND))
    {
        log(Log::ERROR, "",
            "Username does not exists: " + std::string(data[MessageKeys::USERNAME]));

        const LoginMessageResponse login_message_response(Status::ERROR,
                                                          static_cast<UserID>(ErrorCodes::INVALID_USER_ID),
                                                          LoginErrorCodes::USERNAME_NOT_FOUND);
        const nlohmann::json login_response = login_message_response.to_json();

        // const nlohmann::json login_response = {
        //     {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE},
        //     {MessageKeys::STATUS, Status::ERROR},
        //     {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
        //     {MessageKeys::ERROR_CODE, LoginErrorCodes::USERNAME_NOT_FOUND}
        // };

        ws->send(login_response.dump(), uWS::TEXT);
    }
    else if (result == utils::to_underlying(LoginErrorCodes::PASSWORD_IS_INCORRECT))
    {
        log(Log::ERROR, "",
            "Password is incorrect for user: " + std::string(data[MessageKeys::USERNAME]));

        const LoginMessageResponse login_message_response(Status::ERROR,
                                                          static_cast<UserID>(ErrorCodes::INVALID_USER_ID),
                                                          LoginErrorCodes::PASSWORD_IS_INCORRECT);
        const nlohmann::json login_response = login_message_response.to_json();
        // const nlohmann::json login_response = {
        //     {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE},
        //     {MessageKeys::STATUS, Status::ERROR},
        //     {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
        //     {MessageKeys::ERROR_CODE, LoginErrorCodes::PASSWORD_IS_INCORRECT}
        // };

        ws->send(login_response.dump(), uWS::TEXT);
    }
    else
    {
        const LoginMessageResponse login_message_response(Status::SUCCESS,
                                                          result,
                                                          LoginErrorCodes::NONE);
        const nlohmann::json login_response = login_message_response.to_json();
        // nlohmann::json login_response = {
        //     {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGIN_RESPONSE},
        //     {MessageKeys::STATUS, Status::SUCCESS},
        //     {MessageKeys::USER_ID, result},
        //     {MessageKeys::ERROR_CODE, LoginErrorCodes::NONE}
        // };

        // Only one active session is allowed per user across all devices.
        SessionManager *session_manager = SessionManager::instance();
        if (session_manager->is_session_exists(result)) // if session already exists
        {
            Session *session = session_manager->get_session(result);

            const LogoutMessageResponse logout_message_response(Status::SUCCESS);
            const nlohmann::json logout_response = logout_message_response.to_json();

            // const nlohmann::json logout_response = {
            //     {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGOUT_RESPONSE},
            //     {MessageKeys::STATUS, Status::SUCCESS}
            // };

            log(Log::INFO, "",
                "User \'" + std::string(data[MessageKeys::USERNAME]) + "\' logged out forcefully");
            session->ws->send(logout_response.dump(), uWS::TEXT);
            session->ws = ws;
        }
        else
        {
            // new session created
            session_manager->create_session(result, data[MessageKeys::USERNAME], ws);
        }

        SessionManager::instance()->display_sessions(); // debug purpose

        log(Log::INFO, "",
            "User '" + std::string(data[MessageKeys::USERNAME]) + "' logged in successfully");

        ws->send(login_response.dump(), uWS::TEXT);

        send_user_login_payloads(result, ws);
    }
}

void MessageProcessor::process_logout_request(WebSocket *ws,
                                              nlohmann::json &data) const
{
    print_logout_request(data);

    const Status status = _message_handler.logout_request(data);
    if (status == Status::ERROR)
    {
        log(Log::ERROR, "",
            "Failed to log out user \'" + std::string(data[MessageKeys::USERNAME]) + "\'");
    }
    else
    {
        log(Log::INFO, "",
            "User \'" + std::string(data[MessageKeys::USERNAME]) + "\' logged out successfully");
    }

    const LogoutMessageResponse logout_message_response(status);
    const nlohmann::json logout_response = logout_message_response.to_json();

    // const nlohmann::json logout_response = {
    //     {MessageKeys::MESSAGE_TYPE, MessageTypes::LOGOUT_RESPONSE},
    //     {MessageKeys::STATUS, status}
    // };

    ws->send(logout_response.dump(), uWS::TEXT);
}

void MessageProcessor::process_signup_request(WebSocket *ws, nlohmann::json &data) const
{
    // print_signup_request(parsed_message); // debug

    UserID result = _message_handler.signup(data);
    if (result == utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS))
    {
        log(Log::ERROR, "",
            "Username already exists: " + std::string(data[MessageKeys::USERNAME]));

        const nlohmann::json signup_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
            {MessageKeys::STATUS, Status::ERROR},
            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
            {MessageKeys::ERROR_CODE, SignupErrorCodes::USERNAME_ALREADY_EXISTS}
        };

        ws->send(signup_response.dump(), uWS::TEXT);
    }
    else if (result == utils::to_underlying(SignupErrorCodes::EMAIL_ALREADY_EXISTS))
    {
        log(Log::ERROR, "", "Email already exists: " + std::string(data[MessageKeys::EMAIL]));

        const nlohmann::json signup_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
            {MessageKeys::STATUS, Status::ERROR},
            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
            {MessageKeys::ERROR_CODE, SignupErrorCodes::EMAIL_ALREADY_EXISTS}
        };

        ws->send(signup_response.dump(), uWS::TEXT);
    }
    else if (result == utils::to_underlying(SignupErrorCodes::PHONE_ALREADY_EXISTS))
    {
        log(Log::ERROR, "", "Phone already exists: " + std::string(data["phone"]));

        const nlohmann::json signup_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
            {MessageKeys::STATUS, Status::ERROR},
            {MessageKeys::USER_ID, ErrorCodes::INVALID_USER_ID},
            {MessageKeys::ERROR_CODE, SignupErrorCodes::PHONE_ALREADY_EXISTS}
        };

        ws->send(signup_response.dump(), uWS::TEXT);
    }
    else
    {
        const nlohmann::json signup_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::SIGN_UP_RESPONSE},
            {MessageKeys::STATUS, Status::SUCCESS},
            {MessageKeys::USER_ID, result},
            {MessageKeys::ERROR_CODE, SignupErrorCodes::NONE}
        };

        ws->send(signup_response.dump(), uWS::TEXT);
    }
}

void MessageProcessor::process_search_user_request(WebSocket *ws,
                                                   nlohmann::json &data) const
{
    //print_search_user_request(packet_data); // debug

    std::vector<FoundUser> found_users = _message_handler.search_user(data);
    if (found_users.empty())
    {
        const nlohmann::json search_user_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::SEARCH_USER_RESPONSE},
            {MessageKeys::COUNT, 0},
            {MessageKeys::USERS, nlohmann::json::array()}
        };

        ws->send(search_user_response.dump(), uWS::TEXT);
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

        std::cout << "sent: " << search_user_response.dump() << std::endl;
        ws->send(search_user_response.dump(), uWS::TEXT);
    }
}

void MessageProcessor::process_friend_req_request(WebSocket *ws,
                                                  nlohmann::json &data) const
{
    print_friend_req_request(data);

    _message_handler.friend_req_request(data);

    try
    {
        const Session *session = SessionManager::instance()->get_session(
            std::atoi(data[MessageKeys::RECEIVER_ID].dump().c_str()));
        if (session) // session found
        {
            std::cout << data.dump() << std::endl;
            session->ws->send(data.dump(), uWS::TEXT);
        }
    }
    catch (std::exception &e)
    {
        log(Log::ERROR, "", e.what());
    }
}

void MessageProcessor::process_friend_req_response(WebSocket *ws, const nlohmann::json &data) const
{
    print_friend_req_response(data);

    _message_handler.friend_req_response(data);
}

void MessageProcessor::process_change_password_request(WebSocket *ws, const nlohmann::json &data) const
{
    print_change_password_request(data);

    ChangePasswordErrorCodes result = _message_handler.change_password_request(data);
    if (result == ChangePasswordErrorCodes::SOMETHING_WENT_WRONG)
    {
        const nlohmann::json change_password_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::CHANGE_PASSWORD_RESPONSE},
            {MessageKeys::STATUS, Status::ERROR},
            {MessageKeys::ERROR_CODE, result}
        };

        log(Log::ERROR, "", "Something went wrong while changing the password");

        ws->send(change_password_response.dump(), uWS::TEXT);
    }
    else if (result == ChangePasswordErrorCodes::NEW_PASSWORD_MUST_BE_DIFFERENT)
    {
        const nlohmann::json change_password_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::CHANGE_PASSWORD_RESPONSE},
            {MessageKeys::STATUS, Status::ERROR},
            {MessageKeys::ERROR_CODE, result}
        };

        log(Log::ERROR, "", "New password must be different that old password");

        ws->send(change_password_response.dump(), uWS::TEXT);
    }
    else
    {
        const nlohmann::json change_password_response = {
            {MessageKeys::MESSAGE_TYPE, MessageTypes::CHANGE_PASSWORD_RESPONSE},
            {MessageKeys::STATUS, Status::SUCCESS},
            {MessageKeys::ERROR_CODE, result}
        };

        log(Log::ERROR, "", "Password changed successfully");

        ws->send(change_password_response.dump(), uWS::TEXT);
    }
}
