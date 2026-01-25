//
// Created by vaibz on 18/01/26.
//

#ifndef BINGOSERVER_ENUMS_H
#define BINGOSERVER_ENUMS_H
#include <cstdint>

enum class ConversationType : int8_t
{
    PERSONAL = 0,
    GROUP = 1,
    BROADCAST = 2,
};
enum class ContentType : int8_t
{
    TEXT = 0,
    IMAGE = 1,
    VIDEO = 2,
    FILE = 3,
    AUDIO = 4,
    POLL = 5
};

enum class MessageStatus : int8_t
{
    SENT = 0,
    RECEIVED = 1,
    READ = 2
};

#endif //BINGOSERVER_ENUMS_H