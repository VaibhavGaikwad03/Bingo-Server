#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <vector>
#include "message_types.h"
#include <mysqlx/xdevapi.h>
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

public:
    MessageHandler();

    ~MessageHandler();

    [[nodiscard]] UserID login(const nlohmann::json &message) const;
    [[nodiscard]] UserID signup(const nlohmann::json &message) const;
    [[nodiscard]] std::vector<FoundUser> search_user(const nlohmann::json &message) const;
    void friend_request(const nlohmann::json &message) const;
};


#endif //MESSAGE_HANDLER_H
