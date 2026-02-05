//
// Created by vaibz on 15/09/25.
//

#include <fstream>
#include <sstream>
#include "../../include/utils/logger.h"
#include "../../include/nlohmann/json.hpp"
#include "../../include/utils/config_reader.h"

ConfigReader::ConfigReader(const std::string &config_file_path)
{
    std::ifstream config_file(config_file_path);

    if (!config_file.is_open())
    {
        log(Log::CRITICAL, __func__, "Config file not found.");
        return;
    }

    try
    {
        std::ostringstream buffer;
        buffer << config_file.rdbuf();

        nlohmann::json parsed_config = nlohmann::json::parse(buffer.str());

        _ip = parsed_config["ip"].get<std::string>();
        _port = parsed_config["port"].get<int>();
        _debug_flag = parsed_config["debug"].get<bool>();

        // Parse database configuration
        if (parsed_config.contains("database"))
        {
            const auto& db = parsed_config["database"];
            _db_config.host = db["host"].get<std::string>();
            _db_config.port = db["port"].get<int>();
            _db_config.username = db["username"].get<std::string>();
            _db_config.password = db["password"].get<std::string>();
            _db_config.name = db["name"].get<std::string>();
        }
    }
    catch (std::exception &e)
    {
        log(Log::CRITICAL, __func__, e.what());
    }
}

ConfigReader::~ConfigReader()
= default;

[[nodiscard]] std::string ConfigReader::get_ip() const
{
    return _ip;
}

[[nodiscard]] int ConfigReader::get_port() const
{
    return _port;
}

[[nodiscard]] bool ConfigReader::get_debug_flag() const
{
    return _debug_flag;
}

[[nodiscard]] const DatabaseConfig& ConfigReader::get_database_config() const
{
    return _db_config;
}
