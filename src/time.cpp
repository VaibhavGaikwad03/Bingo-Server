#include <ctime>
#include "../include/time.h"

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

    const int year = now->tm_year + 1900;
    const int month = now->tm_mon + 1;
    const int day = now->tm_mday;

    return std::string(std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day));
}
