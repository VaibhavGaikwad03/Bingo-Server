#include "../../include/handlers/GetMessageIdRequestHandler.h"
#include "../../include/message_handler.h"
#include "../../include/debug.h"
#include "../../include/MessageResponseFactory/GetChatMessageIDResponse.h"

void GetMessageIdRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_get_message_id_request(data);

    const MessageID message_id = _message_handler.get_message_id_request();

    const GetChatMessageIDResponse get_chat_message_id_response(message_id);
    const nlohmann::json get_chat_message_response_message = get_chat_message_id_response.to_json();

    // std::string str = get_chat_message_response_message.dump();

    ws->send(get_chat_message_response_message.dump(), uWS::TEXT);
}
