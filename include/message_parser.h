#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include "nlohmann/json.hpp"
#include "message_structures.h"

class MessageParser
{
    MessageParser();

    ~MessageParser();

    // static MessageParser *_instance;

public:
    // static MessageParser *instance();
    // static void destroy_instance();

    MessageParser(const MessageParser &) = delete;
    MessageParser &operator=(const MessageParser &) = delete;

    static std::optional<LoginMessageRequest> login_message_request(const nlohmann::json &message);
    static std::optional<LogoutMessageRequest> logout_message_request(const nlohmann::json &message);
    static std::optional<SignUpMessageRequest> signup_message_request(const nlohmann::json &message);
    static std::optional<SearchUserRequest> search_user_request(const nlohmann::json &message);
    static std::optional<FriendReqRequest> friend_req_request(const nlohmann::json &message);
    static std::optional<FriendReqResponse> friend_req_response(const nlohmann::json &message);
    static std::optional<ChangePassword> change_password_request(const nlohmann::json &message);
    static std::optional<ReconnectRequest> reconnect_request(const nlohmann::json &message);
    static std::optional<UpdateProfileRequest> update_profile_request(const nlohmann::json &message);
};

#endif //MESSAGE_PARSER_H
