#include <ctime>
#include "../../include/utils/time.h"

#include <chrono>

std::string get_time()
{
    time_t now;
    char buffer[20];

    time(&now);
    const struct tm *local_time = localtime(&now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);

    return std::string{buffer};
}

std::string get_date()
{
    const std::time_t current_time = std::time(nullptr);
    const std::tm *now = std::localtime(&current_time);

    const int y = now->tm_year + 1900;
    const int m = now->tm_mon + 1;
    const int d = now->tm_mday;

    const std::string year = std::to_string(y);
    std::string month = std::to_string(m);
    if (month.length() < 2)
    {
        month = "0" + month;
    }

    std::string day = std::to_string(d);
    if (day.length() < 2)
    {
        day = "0" + day;
    }

    return std::string(year + "-" + month + "-" + day);
}
