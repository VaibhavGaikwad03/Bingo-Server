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
    std::string sender_id;
    std::string sender;
    std::string receiver_id;
    std::string receiver;
    std::string timestamp;
};

#endif //MESSAGE_STRUCTURES_H
