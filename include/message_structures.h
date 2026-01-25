//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_STRUCTURES_H
#define MESSAGE_STRUCTURES_H

#include "enums.h"
#include "message_types.h"
#include "uWebSockets/App.h"
#include "typedefs.h"

enum class UserCredentialsTableIndex : uint8_t
{
    USER_ID = 0,
    USERNAME = 1,
    PASSWORD = 2,
    FULLNAME = 3,
    DOB = 4,
    GENDER = 5,
    EMAIL = 6,
    PHONE = 7,
    SIGNUP_TIMESTAMP = 8
};

struct DataPacket
{
    WebSocket *ws;
    std::string data;
    uWS::OpCode opCode;
};

struct LoginMessageRequest
{
    MessageType message_type;
    std::string username;
    std::string password;
    std::string timestamp;
};

struct LogoutMessageRequest
{
    MessageType message_type;
    UserID user_id;
    std::string username;
};

struct SignUpMessageRequest
{
    MessageType message_type;
    std::string username;
    std::string password;
    std::string fullname;
    std::string gender;
    std::string dob;
    std::string email;
    std::string phone;
    std::string timestamp;
};

struct SearchUserRequest
{
    MessageType message_type;
    std::string username;
    std::string requested_by;
};

struct FriendReqRequest
{
    MessageType message_type;
    UserID sender_id;
    std::string sender;
    std::string name_of_sender;
    UserID receiver_id;
    std::string receiver;
    std::string name_of_receiver;
    std::string timestamp;
};

struct FriendReqResponse
{
    MessageType message_type;
    UserID sender_id;
    std::string sender;
    std::string name_of_sender;
    UserID receiver_id;
    std::string receiver;
    std::string name_of_receiver;
    FriendRequestState status;
    std::string timestamp;
};

/// send to the user when logged in
// struct UserProfile
// {
//     std::string name;
//     std::string username;
//     std::string dob;
//     std::string gender;
//     std::string email;
//     std::string phone;
// };

// struct Friend
// {
//     // int user_id;
//     // std::string user;
//     // std::string name_of_user;
//     int friend_id;
//     std::string friend_username;
//     std::string name_of_friend;
// };

// struct PendingFriendRequest
// {
//     int sender_id;
//     std::string sender;
//     std::string name_of_sender;
//     int receiver_id;
//     std::string receiver;
//     std::string name_of_receiver;
//     std::string request_status;
//     std::string timestamp;
// };

struct ChatMessage
{
    MessageType message_type;
    ConversationType conversation_type;
    UserID sender_id;
    UserID receiver_id;
    ContentType content_type;
    std::string content;
    MessageStatus message_status;
    bool is_reply_message;
    MessageID replied_message_id;
};

///

struct ChangePassword
{
    MessageType message_type;
    UserID user_id;
    std::string old_password;
    std::string new_password;
};

struct ReconnectRequest
{
    MessageType message_type;
    UserID user_id;
    std::string auth_token;
};

struct UpdateProfileRequest
{
    MessageType message_type;
    UserID user_id;
    std::string username;
    std::string fullname;
    std::string dob;
    std::string gender;
    std::string email;
    std::string phone;
};

struct GetChatMessageIDRequest
{
    MessageType message_type;
    UserID user_id;
};

#endif //MESSAGE_STRUCTURES_H
