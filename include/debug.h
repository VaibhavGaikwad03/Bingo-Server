#ifndef DEBUG_H
#define DEBUG_H

#include "nlohmann/json.hpp"

void print_login_request(nlohmann::json message);
void print_signup_request(nlohmann::json message);
void print_search_user_request(nlohmann::json message);

#endif //DEBUG_H
