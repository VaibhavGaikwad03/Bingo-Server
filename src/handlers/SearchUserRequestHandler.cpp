#include "../../include/handlers/SearchUserRequestHandler.h"
#include "../../include/utils/logger.h"
#include "../../include/message_handler.h"
#include "../../include/debug.h"

void SearchUserRequestHandler::handle(WebSocket* ws, const nlohmann::json& data) const
{
    print_search_user_request(data); // debug

    const std::optional<SearchUserRequestMessageResponse> search_user_request_message_response = _message_handler.
            search_user_request(data);
    if (!search_user_request_message_response.has_value())
    {
        const SearchUserRequestMessageResponse search_user_request_error_response(0, {});
        const nlohmann::json search_user_response = search_user_request_error_response.to_json();
        ws->send(search_user_response.dump(), uWS::TEXT);
    }
    else
    {
        const nlohmann::json search_user_response = search_user_request_message_response->to_json();

        log(Log::DEBUG, __func__, search_user_response.dump());

        ws->send(search_user_response.dump(), uWS::TEXT);
    }
}
