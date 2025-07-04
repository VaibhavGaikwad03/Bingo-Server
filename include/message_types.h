#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

enum class Status
{
    SUCCESS = 0,
    ERROR = 1,
};

enum class Flag
{
    NO = 0,
    YES = 1
};

enum class FriendRequestStatus
{
    REJECTED = 0,
    ACCEPTED = 1,
};

enum class FriendshipStatus
{
    FRIEND = 0,
    PENDING = 1,
    NOT_FRIEND = 2,
};

enum class MessageTypes
{
    LOGIN_REQUEST = 101,
    LOGIN_RESPONSE = 102,
    SIGN_UP_REQUEST = 103,
    SIGN_UP_RESPONSE = 104,
    LOGOUT_REQUEST = 105,
    LOGOUT_RESPONSE = 106,
    SEARCH_USER_REQUEST = 107,
    SEARCH_USER_RESPONSE = 108,
    FRIEND_REQ_REQUEST = 109,
    FRIEND_REQ_RESPONSE = 110,
};

#endif //MESSAGE_TYPES_H
