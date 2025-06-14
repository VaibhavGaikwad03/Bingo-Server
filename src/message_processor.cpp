//
// Created by vaibz on 11/6/25.
//

#include  "../include/log.h"
#include "../include/nlohmann/json.hpp"
#include "../include/message_processor.h"

MessageProcessor::MessageProcessor(MutexQueue<DataPacket> &queue, std::condition_variable &cv) : _cv(cv),
    _mtx_queue(queue)
{
}

MessageProcessor::~MessageProcessor()
= default;

void MessageProcessor::process()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cv.wait(lock, [this]
        {
            return !_mtx_queue.empty();
        });

        const DataPacket packet = _mtx_queue.dequeue();

        try
        {
            nlohmann::json parsed_message = nlohmann::json::parse(packet.data);

            if (parsed_message["message_type"] == "login_request")
            {
                std::cout << parsed_message["message_type"] << std::endl;
                std::cout << parsed_message["username"] << std::endl;
                std::cout << parsed_message["password"] << std::endl;
                std::cout << parsed_message["timestamp"] << std::endl;
                // packet.ws->send(packet.data, uWS::TEXT);
                // for now it will send response from current thread

                _authenticator.login(parsed_message["username"], parsed_message["password"]);
            }
            else if (parsed_message["MSGTYPE"] == "SIGNIN_REQUEST")
            {
                std::cout << parsed_message["MSGTYPE"] << std::endl;
                std::cout << parsed_message["USERNAME"] << std::endl;
                std::cout << parsed_message["PASSWORD"] << std::endl;
                std::cout << parsed_message["FULLNAME"] << std::endl;
                std::cout << parsed_message["GENDER"] << std::endl;
                std::cout << parsed_message["DOB"] << std::endl;
                std::cout << parsed_message["EMAIL"] << std::endl;
                std::cout << parsed_message["PHONE"] << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            log(Log::ERROR, "", e.what());
        }
    }
}
