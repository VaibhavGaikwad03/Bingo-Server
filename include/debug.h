#ifndef DEBUG_H
#define DEBUG_H

#include "nlohmann/json.hpp"

static void print_key_value(std::string_view key, const nlohmann::json& message);

void print_login_request(const nlohmann::json& message);
void print_logout_request(const nlohmann::json& message);
void print_signup_request(const nlohmann::json& message);
void print_search_user_request(const nlohmann::json& message);
void print_friend_req_request(const nlohmann::json& message);
void print_friend_req_response(const nlohmann::json& message);
void print_change_password_request(const nlohmann::json& message);
void print_reconnect_request(const nlohmann::json& message);
void print_update_profile_request(const nlohmann::json &message);
void print_chat_message(const nlohmann::json &message);
void print_get_message_id_request(const nlohmann::json &message);

#endif //DEBUG_H
