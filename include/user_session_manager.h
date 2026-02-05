#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "user_session.h"
#include <unordered_map>
#include <mutex>
#include "authenticator.h"

class UserSessionManager
{
    mutable std::mutex _mtx;  // Mutex for thread-safe session operations
    std::unordered_map<UserID, UserSession> _sessions;

    UserSessionManager() = default;
    ~UserSessionManager() = default;
public:
    // Meyer's Singleton - thread-safe in C++11
    static UserSessionManager& instance();

    UserSessionManager(const UserSessionManager&) = delete;
    UserSessionManager& operator=(const UserSessionManager&) = delete;

    void create_session(const UserID &user_id, const std::string &username,
                                    WebSocket *ws);

    bool delete_session(UserSession *session);

    void display_sessions() const;

    UserSession* get_session(UserID user_id);
    UserSession* get_session(const std::string &username);
    UserSession* get_session(const WebSocket *ws);

    bool is_session_exists(UserID user_id);
    bool is_session_exists(const std::string &username);
    bool is_session_exists(const WebSocket *ws);

};

#endif //SESSION_MANAGER_H
