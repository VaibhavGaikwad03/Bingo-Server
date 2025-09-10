//
// Created by vaibz on 11/6/25.
//

#include "../include/utils/logger.h"
#include "../include/utils.h"
#include "../include/error_codes.h"
#include "../include/message_types.h"
#include "../include/nlohmann/json.hpp"
#include "../include/message_processor.h"

#include "../include/debug.h"
#include "../include/message_keys.h"
#include "../include/user_session_manager.h"
#include "../include/MessageResponseFactory/ChangePasswordMessageResponse.h"
#include "../include/MessageResponseFactory/LoginMessageResponse.h"
#include "../include/MessageResponseFactory/LogoutMessageResponse.h"
#include "../include/MessageResponseFactory/PendingFriendRequests.h"
#include "../include/MessageResponseFactory/SearchUserRequestMessageResponse.h"
#include "../include/MessageResponseFactory/SignupMessageResponse.h"
#include "../include/MessageResponseFactory/UserProfileMessage.h"


MessageProcessor::MessageProcessor(ThreadSafeQueue<DataPacket> &queue, std::condition_variable &cv) : _cv(cv),
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

            log(Log::DEBUG, __func__, std::string("in process func: ") + packet_data.dump());

            auto message_type = static_cast<MessageType>(packet_data[MessageKeys::MESSAGE_TYPE].get<int>());

            switch (message_type)
            {
                case MessageType::LOGIN_REQUEST:
                {
                    process_login_request(packet.ws, packet_data);
                }
                break;

                case MessageType::LOGOUT_REQUEST:
                {
                    process_logout_request(packet.ws, packet_data);
                }
                break;

                case MessageType::SIGN_UP_REQUEST:
                {
                    process_signup_request(packet.ws, packet_data);
                }
                break;

                case MessageType::SEARCH_USER_REQUEST:
                {
                    process_search_user_request(packet.ws, packet_data);
                }
                break;

                case MessageType::FRIEND_REQ_REQUEST:
                {
                    process_friend_req_request(packet.ws, packet_data);
                }
                break;

                case MessageType::FRIEND_REQ_RESPONSE:
                {
                    process_friend_req_response(packet.ws, packet_data);
                }
                break;

                case MessageType::CHANGE_PASSWORD_REQUEST:
                {
                    process_change_password_request(packet.ws, packet_data);
                }
                break;

                case MessageType::RECONNECT_REQUEST:
                {
                    process_reconnect_request(packet.ws, packet_data);
                }
                break;

                default:
                    log(Log::ERROR, __func__, "Invalid Message Type");
                    break;
            }
        }
        catch (const std::exception &e)
        {
            log(Log::ERROR, __func__, e.what());
        }
    }
}

void MessageProcessor::send_user_login_payloads(const UserID user_id,
                                                WebSocket *ws) const
{
    try
    {
        // FETCH USER PROFILE
        std::optional<UserProfileMessage> user_profile_message = _message_handler.get_user_profile(user_id);
        if (user_profile_message.has_value())
        {
            nlohmann::json user_profile = user_profile_message->to_json();
            log(Log::DEBUG, __func__, user_profile.dump());
            ws->send(user_profile.dump(), uWS::TEXT);
        }

        // FETCH PENDING FRIEND REQUESTS
        std::optional<PendingFriendRequests> pending_friend_requests = _message_handler.
                get_pending_friend_requests(user_id);
        if (pending_friend_requests.has_value())
        {
            nlohmann::json pending_friend_requests_list = pending_friend_requests->to_json();
            log(Log::DEBUG, __func__, pending_friend_requests_list.dump());
            ws->send(pending_friend_requests_list.dump(), uWS::TEXT);
        }

        // FETCH FRIENDS
        std::optional<FriendsListMessage> friends_list_message = _message_handler.get_user_friends(user_id);
        if (friends_list_message.has_value())
        {
            nlohmann::json friends_list = friends_list_message->to_json();
            log(Log::DEBUG, __func__, friends_list.dump());
            ws->send(friends_list.dump(), uWS::TEXT);
        }

        // FETCH MESSAGE HISTORY
    }
    catch (nlohmann::detail::exception &ex)
    {
        log(Log::ERROR, __func__, ex.what());
    }
    catch (std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
    }
}

void MessageProcessor::process_login_request(WebSocket *ws,
                                             nlohmann::json &data) const
{
    // print_login_request(parsed_message); // debug

    // for now, it will send response from current thread
    std::optional<LoginMessageResponse> login_message_response = _message_handler.login_request(data);
    if (login_message_response->get_login_error_codes() == LoginErrorCode::USERNAME_NOT_FOUND)
    {
        log(Log::ERROR, __func__,
            "Username does not exists: " + std::string(data[MessageKeys::USERNAME]));

        const nlohmann::json login_response = login_message_response->to_json();

        ws->send(login_response.dump(), uWS::TEXT);
    }
    else if (login_message_response->get_login_error_codes() == LoginErrorCode::PASSWORD_IS_INCORRECT)
    {
        log(Log::ERROR, __func__,
            "Password is incorrect for user: " + std::string(data[MessageKeys::USERNAME]));

        const nlohmann::json login_response = login_message_response->to_json();

        ws->send(login_response.dump(), uWS::TEXT);
    }
    else
    {
        const nlohmann::json login_response = login_message_response->to_json();

        log(Log::DEBUG, __func__, login_response.dump());

        // Only one active session is allowed per user across all devices.
        UserSessionManager *session_manager = UserSessionManager::instance();
        if (session_manager->is_session_exists(login_message_response->get_userid())) // if session already exists
        {
            UserSession *session = session_manager->get_session(login_message_response->get_userid());

            const LogoutMessageResponse logout_message_response(Status::SUCCESS);
            const nlohmann::json logout_response = logout_message_response.to_json();

            log(Log::INFO, __func__,
                "User \'" + std::string(data[MessageKeys::USERNAME]) + "\' logged out forcefully");
            session->ws->send(logout_response.dump(), uWS::TEXT);
            session->ws = ws;
        }
        else
        {
            // new session created
            session_manager->create_session(login_message_response->get_userid(), data[MessageKeys::USERNAME], ws);
        }

        UserSessionManager::instance()->display_sessions(); // debug purpose

        log(Log::INFO, __func__,
            "User '" + std::string(data[MessageKeys::USERNAME]) + "' logged in successfully");

        ws->send(login_response.dump(), uWS::TEXT);

        send_user_login_payloads(login_message_response->get_userid(), ws);
    }
}

void MessageProcessor::process_logout_request(WebSocket *ws,
                                              nlohmann::json &data) const
{
    print_logout_request(data);

    std::optional<LogoutMessageResponse> logout_message_response = _message_handler.logout_request(data);
    if (!logout_message_response.has_value())
    {
        log(Log::ERROR, __func__,
            "Something went wrong while logging out user \'" + std::string(data[MessageKeys::USERNAME]) + "\'");

        LogoutMessageResponse logout_response_response(Status::ERROR);
        const nlohmann::json logout_response = logout_message_response->to_json();

        log(Log::DEBUG, __func__, logout_response.dump());

        ws->send(logout_response.dump(), uWS::TEXT);

        return;
    }
    else if (logout_message_response->get_status() == Status::ERROR)
    {
        log(Log::ERROR, __func__,
            "Failed to log out user \'" + std::string(data[MessageKeys::USERNAME]) + "\'");
    }
    else
    {
        log(Log::INFO, __func__,
            "User \'" + std::string(data[MessageKeys::USERNAME]) + "\' logged out successfully");
    }

    // const LogoutMessageResponse logout_message_response(status);
    const nlohmann::json logout_response = logout_message_response->to_json();

    log(Log::DEBUG, __func__, logout_response.dump());

    ws->send(logout_response.dump(), uWS::TEXT);
}

void MessageProcessor::process_signup_request(WebSocket *ws, nlohmann::json &data) const
{
    // print_signup_request(parsed_message); // debug

    UserID result = _message_handler.signup_request(data);
    if (result == utils::to_underlying(SignupErrorCode::USERNAME_ALREADY_EXISTS))
    {
        log(Log::ERROR, __func__,
            "Username already exists: " + std::string(data[MessageKeys::USERNAME]));

        const SignupMessageResponse signup_message_response(Status::ERROR,
                                                            static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                            SignupErrorCode::USERNAME_ALREADY_EXISTS);
        const nlohmann::json signup_response = signup_message_response.to_json();

        ws->send(signup_response.dump(), uWS::TEXT);
    }
    else if (result == utils::to_underlying(SignupErrorCode::EMAIL_ALREADY_EXISTS))
    {
        log(Log::ERROR, __func__, "Email already exists: " + std::string(data[MessageKeys::EMAIL]));

        const SignupMessageResponse signup_message_response(Status::ERROR,
                                                            static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                            SignupErrorCode::EMAIL_ALREADY_EXISTS);
        const nlohmann::json signup_response = signup_message_response.to_json();

        ws->send(signup_response.dump(), uWS::TEXT);
    }
    else if (result == utils::to_underlying(SignupErrorCode::PHONE_ALREADY_EXISTS))
    {
        log(Log::ERROR, __func__, "Phone already exists: " + std::string(data["phone"]));

        const SignupMessageResponse signup_message_response(Status::ERROR,
                                                            static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                            SignupErrorCode::PHONE_ALREADY_EXISTS);
        const nlohmann::json signup_response = signup_message_response.to_json();

        ws->send(signup_response.dump(), uWS::TEXT);
    }
    else
    {
        const SignupMessageResponse signup_message_response(Status::SUCCESS,
                                                            result,
                                                            SignupErrorCode::NONE);
        const nlohmann::json signup_response = signup_message_response.to_json();

        ws->send(signup_response.dump(), uWS::TEXT);
    }
}

void MessageProcessor::process_search_user_request(WebSocket *ws,
                                                   nlohmann::json &data) const
{
    print_search_user_request(data); // debug

    std::vector<FoundUser> found_users = _message_handler.search_user(data);
    if (found_users.empty())
    {
        const SearchUserRequestMessageResponse search_user_request_response(0, nlohmann::json::array());
        const nlohmann::json search_user_response = search_user_request_response.to_json();

        ws->send(search_user_response.dump(), uWS::TEXT);
    }
    else
    {
        nlohmann::json users = nlohmann::json::array();

        for (const auto &found_user: found_users)
        {
            // search_user_response[MessageKeys::USERS].push_back({
            users.push_back({
                {MessageKeys::USER_ID, found_user.user_id},
                {MessageKeys::USERNAME, found_user.username},
                {MessageKeys::DISPLAY_NAME, found_user.name},
                {MessageKeys::FRIENDSHIP_STATUS, found_user.friendship_status}
            });
        }

        const SearchUserRequestMessageResponse
                search_user_request_response(static_cast<int>(found_users.size()), users);
        const nlohmann::json search_user_response = search_user_request_response.to_json();

        log(Log::DEBUG, __func__, search_user_response.dump());

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
        const UserSession *session = UserSessionManager::instance()->get_session(
            std::atoi(data[MessageKeys::RECEIVER_ID].dump().c_str()));
        if (session) // session found
        {
            session->ws->send(data.dump(), uWS::TEXT);
        }
    }
    catch (std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
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

    ChangePasswordErrorCode result = _message_handler.change_password_request(data);
    if (result == ChangePasswordErrorCode::SOMETHING_WENT_WRONG)
    {
        const ChangePasswordResponse change_password_response(Status::ERROR, result);
        const nlohmann::json change_password = change_password_response.to_json();

        log(Log::ERROR, __func__, "Something went wrong while changing the password");

        ws->send(change_password.dump(), uWS::TEXT);
    }
    else if (result == ChangePasswordErrorCode::NEW_PASSWORD_MUST_BE_DIFFERENT)
    {
        const ChangePasswordResponse change_password_response(Status::ERROR, result);
        const nlohmann::json change_password = change_password_response.to_json();

        log(Log::ERROR, __func__, "New password must be different that old password");

        ws->send(change_password.dump(), uWS::TEXT);
    }
    else
    {
        const ChangePasswordResponse change_password_response(Status::SUCCESS, result);
        const nlohmann::json change_password = change_password_response.to_json();

        log(Log::ERROR, __func__, "Password changed successfully");

        ws->send(change_password.dump(), uWS::TEXT);
    }
}

void MessageProcessor::process_reconnect_request(WebSocket *ws, const nlohmann::json &data) const
{
    print_reconnect_request(data);

    const std::optional<ReconnectResponse> reconnect_response = _message_handler.reconnect_request(data);
    if (!reconnect_response.has_value())
    {
        const ReconnectResponse reconnect_response_error(Status::ERROR, ReconnectErrorCode::SOMETHING_WENT_WRONG);
        const nlohmann::json reconnect_response_error_message = reconnect_response_error.to_json();

        ws->send(reconnect_response_error_message.dump(), uWS::TEXT);
    }

    const UserID user_id = data[MessageKeys::USER_ID].get<UserID>();

    if (reconnect_response->get_status() == Status::SUCCESS)
    {
        const nlohmann::json reconnect_response_message = reconnect_response->to_json();

        ws->send(reconnect_response_message.dump(), uWS::TEXT);

        send_user_login_payloads(user_id, ws);

        log(Log::ERROR, __func__, "Reconnect successful of user id: " + std::to_string(user_id));
    }
    else
    {
        const nlohmann::json reconnect_response_message = reconnect_response->to_json();

        ws->send(reconnect_response_message.dump(), uWS::TEXT);

        log(Log::ERROR, __func__, "Reconnect unsuccessful of user id: " + std::to_string(user_id));
    }
}
