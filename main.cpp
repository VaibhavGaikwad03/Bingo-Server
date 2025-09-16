#include <fstream>

#include "include/file_paths.h"
#include "include/SignalHandler/handle_signals.h"
#include "include/utils/logger.h"
#include "include/server.h"
#include "include/utils/config_reader.h"

int main()
{
    try
    {
        const ConfigReader config_reader(CONFIG_FILE_PATH);

        const std::string ip = config_reader.get_ip();
        const int port = config_reader.get_port();

        Server server(ip, port);

        // init signal handlers
        init_signal_handler();

        server.run();
    }
    catch (const std::exception &e)
    {
        log(Log::CRITICAL, __func__, e.what());
    }

    return 0;
}
