#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "session.h"
#include <unordered_map>
#include "authenticator.h"

class SessionManager
{
    static SessionManager *_instance;
    std::unordered_map<UserID, Session> _sessions;

    SessionManager();
    ~SessionManager();
public:
    static SessionManager *instance();
    static void destroy_instance();

    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

    void create_session(const UserID &user_id, const std::string &username,
                                    uWS::WebSocket<false, uWS::SERVER, std::string> *ws);

    void delete_session(UserID user_id);
    void delete_session(const uWS::WebSocket<false, uWS::SERVER, std::string> *ws);

    void display_sessions() const;

    Session* get_session(UserID user_id);
    Session* get_session(const std::string &username);
    Session* get_session(uWS::WebSocket<false, uWS::SERVER, std::string> *ws);
};

#endif //SESSION_MANAGER_H
