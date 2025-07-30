//
// Created by vaibz on 7/6/25.
//

#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include "../include/mutex_queue.h"
#include "../include/uWebSockets/App.h"
#include "../include/message_processor.h"
#include "../include/message_structures.h"
#include "session.h"

// struct UserData
// {
//     std::string packet;
// };

// typedef int UserID;

class Server
{
    std::string _ip;
    int _port;

    uWS::App _app;
    MutexQueue<DataPacket> _mtx_queue;
    // MessageProcessor *_message_processor;
    std::unique_ptr<MessageProcessor> _message_processor;

    std::unordered_map<UserID, Session> _sessions; // when user login create the session for that user? when use logout remove the session for that user?
    // issue: client1 sends message to client2, pn tyaveli client2 offline ahe. mg client2 la send kelele messages kuthe store karayche??
    // jr aapn te messages client2 chya session struct madhe store karaych tharavla tr tyach session tr atta nasanar ahe karan client2 offline ahe mg kay karava lagel??
    // ek mysql madhe table create kr offline_messages cha tyamdhe je clients offline ahet tyanchyasathiche messages store kr
    // jyaveli user login karel tyaveli offline_messages chya table madhe check kr ki tya client sathi konte messages ahet ka?? ani astil tr tya client la send kr

    // logic for message sent, message deliver, message seen: client1 sends chat message to client2. jevha client1 cha message
    // server kade jail tevha server ek response pathvel client1 la ki message send zala ahe, message jevha deliver hoil client2 la tevha
    // tevha pn server client1 la ek reponse pathvel ki message deliver zala ahe.
    // ajun ek bhari idea ahe.. chat_message wala format ahe tyamdhe message_status field add karaychi tyamadhe sagla message_status
    // store karaych.

    std::condition_variable _cv; // for notifying the message processor thread

    static void connection_opened(WebSocket *ws);

    static void connection_closed(const WebSocket *ws, int code,
                                  std::string_view reason);

    void message_received(WebSocket *ws, std::string_view data,
                          uWS::OpCode opCode);

    static std::string get_websocket_close_reason(int code);

public:
    Server(const std::string& ip, int port);

    ~Server();

    void run();
};

#endif // SERVER_H
