#include "../include/utils/log.h"
#include "../include/server.h"
#include "../include/nlohmann/json.hpp"
#include "../include/message_processor.h"
#include "../include/message_structures.h"
#include "../include/session_manager.h"

Server::Server(const std::string &ip, const int port)
{
    _ip = ip;
    _port = port;

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

    _app.listen(_ip, _port, [this](const auto *token)
    {
        if (token)
        {
            log(Log::INFO, "", "Listening on port: " + std::to_string(_port));
            std::cout << "Listening on port: " + std::to_string(_port) << std::endl;
        }
        else
        {
            log(Log::CRITICAL, "", "Failed to start server on port: " + std::to_string(_port));
            std::cerr << "Failed to start server on port: " + std::to_string(_port) << std::endl;
        }
    }).run();

    message_processor_thread.join();
}

void Server::connection_opened(WebSocket *ws)
{
    log(Log::INFO, "", "Client connected with IP: " + std::string(ws->getRemoteAddressAsText()));
}

void Server::connection_closed(const WebSocket *ws, const int code,
                               const std::string_view reason)
{
    Session *session = SessionManager::instance()->get_session(ws);
    if (session == nullptr)
        return;

    SessionManager::instance()->delete_session(session); // if user disconnects, destroy the session
    SessionManager::instance()->display_sessions(); // debug purpose
    log(Log::INFO, "", "Client disconnected. " + get_websocket_close_reason(code) + ", Reason: " + std::string(reason));
}

void Server::message_received(WebSocket *ws, const std::string_view data,
                              const uWS::OpCode opCode)
{
    log(Log::DEBUG, "", "Message received from client " + std::string(data));

    const DataPacket data_packet = {ws, std::string(data), opCode};

    _mtx_queue.enqueue(data_packet);

    _cv.notify_one();
}

std::string Server::get_websocket_close_reason(const int code)
{
    switch (code)
    {
        case 1000:
            return "Normal closure";
        case 1001:
            return "Client is going away";
        case 1002:
            return "Protocol error";
        case 1003:
            return "Unsupported data";
        case 1005:
            return "No status received (no close frame)";
        case 1006:
            return "Abnormal closure (no close frame received)";
        case 1007:
            return "Invalid payload data";
        case 1008:
            return "Policy violation";
        case 1009:
            return "Message too big";
        case 1010:
            return "Extension required";
        case 1011:
            return "Internal server error";
        case 1012:
            return "Service restart";
        case 1013:
            return "Try again later";
        case 1014:
            return "Bad gateway";
        case 1015:
            return "TLS handshake failed";
        default:
            return "Unknown or reserved code";
    }
}
