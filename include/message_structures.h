//
// Created by vaibz on 11/6/25.
//

#ifndef MESSAGE_STRUCTURES_H
#define MESSAGE_STRUCTURES_H

#include "message_types.h"
#include "uWebSockets/App.h"

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
    uWS::WebSocket<false, uWS::SERVER, std::string> *ws;
    std::string_view data;
    uWS::OpCode opCode;
};

struct LoginMessageRequest
{
    MessageTypes message_type;
    std::string username;
    std::string password;
    std::string timestamp;
};

struct SignUpMessageRequest
{
    MessageTypes message_type;
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
    MessageTypes message_type;
    std::string username;
    std::string requested_by;
};

struct FriendReqRequest
{
    MessageTypes message_type;
    int sender_id;
    std::string sender;
    int receiver_id;
    std::string receiver;
    std::string timestamp;
};

/// send to the user when logged in
struct UserProfile
{
    std::string name;
    std::string username;
    std::string dob;
    std::string gender;
    std::string email;
    std::string phone;
};

struct Friend
{
    // int user_id;
    // std::string user;
    // std::string name_of_user;
    int friend_id;
    std::string friend_username;
    std::string name_of_friend;
};

struct PendingFriendRequest
{
    int sender_id;
    std::string sender;
    std::string sender_name;
    int receiver_id;
    std::string receiver;
    std::string request_status;
    std::string timestamp;
};

struct ChatMessage
{
    int sender_id;
    std::string sender;
    int receiver_id;
    std::string receiver;
    std::string message;
    std::string message_status;
    std::string timestamp;
};
///

#endif //MESSAGE_STRUCTURES_H
