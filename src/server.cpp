#include "../include/log.h"
#include "../include/server.h"
#include "../include/nlohmann/json.hpp"
#include "../include/message_processor.h"
#include "../include/message_structures.h"
#include "../include/session_manager.h"

Server::Server()
{
    _message_processor = std::make_unique<MessageProcessor>(_mtx_queue, _cv);

    uWS::App::WebSocketBehavior<std::string> behavior;
    behavior.open = [this](auto *ws)
    {
        Server::connection_opened(ws);
    };

    behavior.close = [this](auto *ws, int code, std::string_view reason)
    {
        Server::connection_closed(ws, code, reason);
    };

    behavior.message = [this](auto *ws, std::string_view data, uWS::OpCode op_code)
    {
        this->message_received(ws, data, op_code);
    };

    _app.ws<std::string>("/*", std::move(behavior));

    log(Log::INFO, "", "Server started...");
}

Server::~Server()
{
    log(Log::INFO, "", "Server stopped...");
}

void Server::run()
{
    std::thread message_processor_thread([this]()
    {
        _message_processor->process();
    });

    _app.listen(2121, [](const auto *token)
    {
        if (token)
        {
            std::cout << "Server listening on port: 2121" << std::endl;
        }
        else
        {
            std::cout << "Server stopped" << std::endl;
        }
    }).run();

    message_processor_thread.join();
}

void Server::connection_opened(uWS::WebSocket<false, uWS::SERVER, std::string> *ws)
{
    log(Log::INFO, "", "Client connected with IP: " + std::string(ws->getRemoteAddressAsText()));
}

void Server::connection_closed(uWS::WebSocket<false, uWS::SERVER, std::string> *ws, const int code,
                               const std::string_view reason)
{
    SessionManager::instance()->delete_session(ws); // if user disconnects, it destroys the session
    SessionManager::instance()->display_sessions(); // debug purpose
    log(Log::INFO, "", "Client disconnected. Code: " + std::to_string(code) + ", Reason: " + std::string(reason));
}

void Server::message_received(uWS::WebSocket<false, uWS::SERVER, std::string> *ws, std::string_view data,
                              uWS::OpCode opCode)
{
    std::cout << data << std::endl; // debug

    const DataPacket data_packet = {ws, data, opCode};

    _mtx_queue.enqueue(data_packet);

    _cv.notify_one();
}
