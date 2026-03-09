#include "../../include/handlers/HandlerUtils.h"
#include "../../include/message_handler.h"
#include "../../include/utils/logger.h"
#include "../../include/MessageResponseFactory/ChatMessageResponse.h"

namespace HandlerUtils
{
    void send_user_login_payloads(const UserID user_id, WebSocket* ws, const MessageHandler& message_handler)
    {
        try
        {
            // FETCH USER PROFILE
            std::optional<UserProfileMessage> user_profile_message = message_handler.get_user_profile(user_id);
            if (user_profile_message.has_value())
            {
                nlohmann::json user_profile = user_profile_message->to_json();
                log(Log::DEBUG, __func__, user_profile.dump());
                ws->send(user_profile.dump(), uWS::TEXT);
            }

            // FETCH PENDING FRIEND REQUESTS
            std::optional<PendingFriendRequests> pending_friend_requests = message_handler.get_pending_friend_requests(user_id);
            if (pending_friend_requests.has_value())
            {
                nlohmann::json pending_friend_requests_list = pending_friend_requests->to_json();
                log(Log::DEBUG, __func__, pending_friend_requests_list.dump());
                ws->send(pending_friend_requests_list.dump(), uWS::TEXT);
            }

            // FETCH FRIENDS
            std::optional<FriendsListMessage> friends_list_message = message_handler.get_user_friends(user_id);
            if (friends_list_message.has_value())
            {
                nlohmann::json friends_list = friends_list_message->to_json();
                log(Log::DEBUG, __func__, friends_list.dump());
                ws->send(friends_list.dump(), uWS::TEXT);
            }

            // FETCH MESSAGE HISTORY
            std::optional<ChatHistoryListMessage> chat_history_list = message_handler.get_chat_messages(user_id);
            if (chat_history_list.has_value())
            {
                nlohmann::json chat_history_list_json = chat_history_list->to_json();
                log(Log::DEBUG, __func__, chat_history_list_json.dump());
                ws->send(chat_history_list_json.dump(), uWS::TEXT);
            }
        }
        catch (nlohmann::detail::exception &ex)
        {
            log(Log::ERROR, __func__, ex.what());
        }
        catch (std::exception &e)
        {
            log(Log::ERROR, __func__, e.what());
        }
    }
}
