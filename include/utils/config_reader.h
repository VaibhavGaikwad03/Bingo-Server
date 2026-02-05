//
// Created by vaibz on 15/09/25.
//

#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#include <string>

struct DatabaseConfig
{
    std::string host;
    int port;
    std::string username;
    std::string password;
    std::string name;
};

class ConfigReader
{
    std::string _ip;
    int _port;
    bool _debug_flag;
    DatabaseConfig _db_config;

public:
    ConfigReader(const std::string &config_file_path);
    ~ConfigReader();

    [[nodiscard]] std::string get_ip() const;
    [[nodiscard]] int get_port() const;
    [[nodiscard]] bool get_debug_flag() const;
    [[nodiscard]] const DatabaseConfig& get_database_config() const;
};

#endif // CONFIG_READER_H
