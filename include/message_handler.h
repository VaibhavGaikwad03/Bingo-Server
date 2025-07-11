#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <vector>
#include "message_types.h"
#include <mysqlx/xdevapi.h>

// #include "message_processor.h"
#include "message_structures.h"
#include "nlohmann/json.hpp"

typedef int UserID;

struct FoundUser
{
    UserID user_id;
    std::string name;
    std::string username;
    Flag is_friend;
};

class MessageHandler
{
    std::unique_ptr<mysqlx::Session> _session;
    std::unique_ptr<mysqlx::Schema> _database;

    std::unique_ptr<mysqlx::Table> _friendship_table;
    std::unique_ptr<mysqlx::Table> _friend_request_table;
    std::unique_ptr<mysqlx::Table> _message_history_table;
    std::unique_ptr<mysqlx::Table> _user_credentials_table;

    [[nodiscard]] bool has_pending_friend_request(int sender_id, int receiver_id) const;
    std::optional<UserProfile> get_user_profile(UserID user_id) const;
    std::vector<Friend> get_user_friends(UserID user_id) const;
    std::vector<PendingFriendRequest> get_pending_friend_requests(UserID user_id) const;
    std::vector<ChatMessage> get_chat_messages(UserID user_id);

public:
    MessageHandler();

    ~MessageHandler();

    [[nodiscard]] UserID login(const nlohmann::json &message) const;
    [[nodiscard]] UserID signup(const nlohmann::json &message) const;
    [[nodiscard]] std::vector<FoundUser> search_user(const nlohmann::json &message) const;
    void friend_req_request(const nlohmann::json &message) const;

    // friend void MessageProcessor::send_user_login_payloads();
};


#endif //MESSAGE_HANDLER_H
