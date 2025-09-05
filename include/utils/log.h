#ifndef ERROR_H
#define ERROR_H

#include <string>

enum class Log
{
    ERROR = 0,
    INFO = 1,
    WARNING = 2,
    CRITICAL = 3,
    DEBUG = 4
};

void log(Log log_type, const std::string& function_name, const std::string& message);

#endif // ERROR_H