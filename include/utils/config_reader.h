//
// Created by vaibz on 15/09/25.
//

#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#include <string>

class ConfigReader
{
    std::string _ip;
    int _port;
    bool _debug_flag;

public:
    ConfigReader(const std::string &config_file_path);
    ~ConfigReader();

    [[nodiscard]] std::string get_ip() const;
    [[nodiscard]] int get_port() const;
    [[nodiscard]] bool get_debug_flag() const;
};

#endif // CONFIG_READER_H
