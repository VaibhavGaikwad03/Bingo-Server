#include <fstream>
#include "include/log.h"
#include "include/server.h"

int main()
{
    std::ifstream config_file("/home/vaibz/Desktop/BingoServer/config/server_config.json");

    if (!config_file.is_open())
    {
        log(Log::CRITICAL, __func__, "Server config file not found");
        return -1;
    }

    try
    {
        std::ostringstream buffer;
        buffer << config_file.rdbuf();

        nlohmann::json parsed_config = nlohmann::json::parse(buffer.str());

        std::string ip = parsed_config["ip"];
        int port = parsed_config["port"];

        Server server(ip, port);

        server.run();
    }
    catch (const std::exception& e)
    {
        log(Log::CRITICAL, __func__, e.what());
    }

    return 0;
}
