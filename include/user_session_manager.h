#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "user_session.h"
#include <string_view>
#include <unordered_map>
#include <mutex>
#include "authenticator.h"

// Thread-safe registry of online users.
//
// All access to the session maps happens while _mtx is held; no pointer or
// reference to a UserSession ever escapes the lock. Callers that need to act on
// a session (send a payload, replace its socket) do so through the operation
// methods below, which perform the work under the lock. This removes the
// use-after-free that existed when raw UserSession* were returned to callers.
//
// A reverse index (_ws_index) maps each active socket back to its user id so
// that disconnect handling is O(1) instead of a linear scan over all sessions.
class UserSessionManager
{
    mutable std::mutex _mtx;  // guards _sessions and _ws_index
    std::unordered_map<UserID, UserSession> _sessions;
    std::unordered_map<const WebSocket*, UserID> _ws_index;

    UserSessionManager() = default;
    ~UserSessionManager() = default;

    // Removes ws from the reverse index. Caller must hold _mtx.
    void unindex_ws(const WebSocket* ws);

public:
    // Meyer's Singleton - thread-safe in C++11
    static UserSessionManager& instance();

    UserSessionManager(const UserSessionManager&) = delete;
    UserSessionManager& operator=(const UserSessionManager&) = delete;

    // Attaches ws to user_id. If a session already exists, its previous socket
    // is sent kick_payload (forced logout) and then replaced with ws; otherwise
    // a new session is created. Performed atomically under the lock.
    void attach_session(const UserID& user_id, const std::string& username,
                        WebSocket* ws, std::string_view kick_payload);

    // Sends payload to the user's active socket. Returns true if a session
    // existed (and the send was issued), false otherwise.
    bool send_to_user(const UserID& user_id, std::string_view payload) const;

    // Removes a session by user id (logout). Returns true if one existed.
    bool remove_session(const UserID& user_id);

    // Removes a session by socket (disconnect). Returns true if one existed.
    bool remove_session_by_ws(const WebSocket* ws);

    [[nodiscard]] bool is_online(const UserID& user_id) const;

    void display_sessions() const;
};

#endif //SESSION_MANAGER_H
