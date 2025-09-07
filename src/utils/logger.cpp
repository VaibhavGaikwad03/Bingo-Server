#include <fstream>
#include <iostream>
#include <filesystem>

#include "../../include/utils/logger.h"
#include "../../include/utils/time.h"

void log(Log log_type, const std::string &function_name, const std::string &message)
{
    std::string log_type_str, current_time;
    std::filesystem::path fs_log_file_path = std::string("../logs/") + get_date() + std::string(".log");

    std::filesystem::create_directories(fs_log_file_path.parent_path());

    std::ofstream log_file(fs_log_file_path, std::ios::app);

    if (!log_file.is_open())
    {
        std::cerr << "Failed to open log file: " << fs_log_file_path << std::endl;
        return;
    }

    switch (log_type)
    {
        case Log::ERROR:
            log_type_str = "ERROR";
            break;

        case Log::INFO:
            log_type_str = "INFO";
            break;

        case Log::WARNING:
            log_type_str = "WARNING";
            break;

        case Log::CRITICAL:
            log_type_str = "CRITICAL";
            break;

        case Log::DEBUG:
            log_type_str = "DEBUG";
            break;

        default:
            log_type_str = "UNKNOWN";
            break;
    }

    current_time = get_time();

    if (!function_name.empty())
        log_file << "[" << current_time << "]" << "[" << log_type_str << "][" << function_name << "] " << message <<
                std::endl;
    else
        log_file << "[" << current_time << "]" << "[" << log_type_str << "]" << function_name << " " << message <<
                std::endl;

    log_file.close();
}
