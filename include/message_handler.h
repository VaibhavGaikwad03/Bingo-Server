#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <vector>
#include <mysqlx/xdevapi.h>

#include "message_structures.h"
#include "nlohmann/json.hpp"
#include "typedefs.h"
#include "error_codes.h"
#include "utils/config_reader.h"
#include "MessageResponseFactory/LoginMessageResponse.h"
#include "MessageResponseFactory/ReconnectMessageResponse.h"
#include "MessageResponseFactory/FriendsListMessage.h"
#include "MessageResponseFactory/PendingFriendRequests.h"
#include "MessageResponseFactory/UserProfileMessage.h"
#include "MessageResponseFactory/LogoutMessageResponse.h"
#include "MessageResponseFactory/SignupMessageResponse.h"
#include "MessageResponseFactory/SearchUserRequestMessageResponse.h"
#include "MessageResponseFactory/UpdateProfileResponse.h"

class MessageHandler
{
    std::unique_ptr<mysqlx::Session> _session;
    std::unique_ptr<mysqlx::Schema> _database;

    std::unique_ptr<mysqlx::Table> _friendship_table;
    std::unique_ptr<mysqlx::Table> _friend_request_table;
    std::unique_ptr<mysqlx::Table> _user_credentials_table;
    std::unique_ptr<mysqlx::Table> _auth_tokens_table;
    std::unique_ptr<mysqlx::Table> _chat_history_table;

    // Inline enum-to-string converters for database storage
    [[nodiscard]] static inline std::string conversation_type_to_string(ConversationType type) 
    {
        switch (type) 
        {
            case ConversationType::PERSONAL: 
                return "personal";
            case ConversationType::GROUP: 
                return "group";
            case ConversationType::BROADCAST: 
                return "broadcast";
            default: 
                return "personal";
        }
    }

    [[nodiscard]] static inline std::string content_type_to_string(ContentType type) 
    {
        switch (type) 
        {
            case ContentType::TEXT: 
                return "text";
            case ContentType::IMAGE: 
                return "image";
            case ContentType::VIDEO: 
                return "video";
            case ContentType::FILE: 
                return "file";
            case ContentType::AUDIO: 
                return "audio";
            case ContentType::POLL: 
                return "poll";
            default: 
                return "text";
        }
    }

    [[nodiscard]] static inline std::string message_status_to_string(MessageStatus status) 
    {
        switch (status) 
        {
            case MessageStatus::SENT: 
                return "sent";
            case MessageStatus::RECEIVED: 
                return "received";
            case MessageStatus::READ: 
                return "read";
            default: 
                return "sent";
        }
    }

    [[nodiscard]] bool has_pending_friend_request(int sender_id, int receiver_id) const;
    [[nodiscard]] std::optional<UserProfileMessage> get_user_profile(UserID user_id) const;
    [[nodiscard]] std::optional<FriendsListMessage> get_user_friends(UserID user_id) const;
    [[nodiscard]] std::optional<PendingFriendRequests> get_pending_friend_requests(UserID user_id) const;
    std::vector<ChatMessage> get_chat_messages(UserID user_id);

    // getters
    [[nodiscard]] std::string get_username(UserID user_id) const;
    [[nodiscard]] std::string get_fullname(UserID user_id) const;
    [[nodiscard]] std::string get_dob(UserID user_id) const;
    [[nodiscard]] std::string get_gender(UserID user_id) const;
    [[nodiscard]] std::string get_email(UserID user_id) const;
    [[nodiscard]] std::string get_phone(UserID user_id) const;

public:
    explicit MessageHandler(const DatabaseConfig& db_config);

    ~MessageHandler();

    [[nodiscard]] std::optional<LoginMessageResponse> login_request(const nlohmann::json &message) const;
    [[nodiscard]] std::optional<LogoutMessageResponse> logout_request(const nlohmann::json &message) const;
    [[nodiscard]] std::optional<SignupMessageResponse> signup_request(const nlohmann::json &message) const;
    [[nodiscard]] std::optional<SearchUserRequestMessageResponse> search_user_request(const nlohmann::json &message) const;
    void friend_req_request(const nlohmann::json &message) const;
    void friend_req_response(const nlohmann::json &message) const;
    [[nodiscard]] ChangePasswordErrorCode change_password_request(const nlohmann::json &message) const;
    [[nodiscard]] std::optional<ReconnectResponse> reconnect_request(const nlohmann::json &message) const;
    [[nodiscard]] std::optional<UpdateProfileResponse> update_profile_request(const nlohmann::json &message) const;
    void chat_message(const nlohmann::json &message) const;
    [[nodiscard]] MessageID get_message_id_request() const;

    friend class MessageProcessor;
};


#endif //MESSAGE_HANDLER_H
