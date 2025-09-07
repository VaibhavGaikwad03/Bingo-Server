#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <vector>
#include "message_types.h"
#include <mysqlx/xdevapi.h>

#include "message_structures.h"
#include "nlohmann/json.hpp"
#include "typedefs.h"
#include "error_codes.h"
#include "MessageResponseFactory/LoginMessageResponse.h"
#include "MessageResponseFactory/ReconnectMessageResponse.h"
#include "MessageResponseFactory/FriendsListMessage.h"
#include "MessageResponseFactory/PendingFriendRequests.h"
#include "../include/MessageResponseFactory/UserProfileMessage.h"

struct FoundUser
{
    UserID user_id;
    std::string name;
    std::string username;
    FriendshipState friendship_status;
};

class MessageHandler
{
    std::unique_ptr<mysqlx::Session> _session;
    std::unique_ptr<mysqlx::Schema> _database;

    std::unique_ptr<mysqlx::Table> _friendship_table;
    std::unique_ptr<mysqlx::Table> _friend_request_table;
    std::unique_ptr<mysqlx::Table> _message_history_table;
    std::unique_ptr<mysqlx::Table> _user_credentials_table;
    std::unique_ptr<mysqlx::Table> _auth_tokens_table;

    [[nodiscard]] bool has_pending_friend_request(int sender_id, int receiver_id) const;
    [[nodiscard]] std::optional<UserProfileMessage> get_user_profile(UserID user_id) const;
    [[nodiscard]] std::optional<FriendsListMessage> get_user_friends(UserID user_id) const;
    [[nodiscard]] std::optional<PendingFriendRequests> get_pending_friend_requests(UserID user_id) const;
    std::vector<ChatMessage> get_chat_messages(UserID user_id);

public:
    MessageHandler();

    ~MessageHandler();

    [[nodiscard]] std::optional<LoginMessageResponse> login(const nlohmann::json &message) const;
    [[nodiscard]] Status logout_request(const nlohmann::json &message) const;
    [[nodiscard]] UserID signup(const nlohmann::json &message) const;
    [[nodiscard]] std::vector<FoundUser> search_user(const nlohmann::json &message) const;
    void friend_req_request(const nlohmann::json &message) const;
    void friend_req_response(const nlohmann::json &message) const;
    [[nodiscard]] ChangePasswordErrorCode change_password_request(const nlohmann::json &message) const;
    [[nodiscard]] std::optional<ReconnectResponse> reconnect_request(const nlohmann::json &message) const;


    friend class MessageProcessor;
};


#endif //MESSAGE_HANDLER_H
