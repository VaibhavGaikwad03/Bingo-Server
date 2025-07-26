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
                                    WebSocket *ws);

    bool delete_session(Session *session);

    void display_sessions() const;

    Session* get_session(UserID user_id);
    Session* get_session(const std::string &username);
    Session* get_session(const WebSocket *ws);

    bool is_session_exists(UserID user_id);
    bool is_session_exists(const std::string &username);
    bool is_session_exists(const WebSocket *ws);

};

#endif //SESSION_MANAGER_H
