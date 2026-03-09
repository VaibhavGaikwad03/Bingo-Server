#include "../../include/handlers/FriendReqResponseHandler.h"
#include "../../include/message_handler.h"
#include "../../include/debug.h"

void FriendReqResponseHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_friend_req_response(data);

    _message_handler.friend_req_response(data);
}
