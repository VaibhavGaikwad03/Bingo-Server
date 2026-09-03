#include "../include/user_session_manager.h"

#include <iostream>

UserSessionManager& UserSessionManager::instance()
{
    static UserSessionManager instance;  // Thread-safe in C++11 (Meyer's Singleton)
    return instance;
}

void UserSessionManager::unindex_ws(const WebSocket* ws)
{
    // Caller holds _mtx.
    if (ws != nullptr)
    {
        _ws_index.erase(ws);
    }
}

void UserSessionManager::attach_session(const UserID& user_id, const std::string& username,
                                        WebSocket* ws, const std::string_view kick_payload)
{
    std::lock_guard<std::mutex> lock(_mtx);

    const auto it = _sessions.find(user_id);
    if (it == _sessions.end())
    {
        // Brand-new session.
        UserSession session;
        session.user_id = user_id;
        session.username = username;
        session.ws = ws;
        session.reference_count = 1;

        _sessions[user_id] = session;
        _ws_index[ws] = user_id;
        return;
    }

    // Existing session: kick the previous socket and take over.
    UserSession& session = it->second;
    if (session.ws != nullptr && session.ws != ws)
    {
        session.ws->send(std::string(kick_payload), uWS::TEXT);
        unindex_ws(session.ws);
    }
    session.ws = ws;
    _ws_index[ws] = user_id;
}

bool UserSessionManager::send_to_user(const UserID& user_id, const std::string_view payload) const
{
    std::lock_guard<std::mutex> lock(_mtx);

    const auto it = _sessions.find(user_id);
    if (it == _sessions.end() || it->second.ws == nullptr)
    {
        return false;
    }

    it->second.ws->send(std::string(payload), uWS::TEXT);
    return true;
}

bool UserSessionManager::remove_session(const UserID& user_id)
{
    std::lock_guard<std::mutex> lock(_mtx);

    const auto it = _sessions.find(user_id);
    if (it == _sessions.end())
    {
        return false;
    }

    if (it->second.reference_count > 1)
    {
        it->second.reference_count--;
    }
    else
    {
        unindex_ws(it->second.ws);
        _sessions.erase(it);
    }
    return true;
}

bool UserSessionManager::remove_session_by_ws(const WebSocket* ws)
{
    std::lock_guard<std::mutex> lock(_mtx);

    const auto idx = _ws_index.find(ws);
    if (idx == _ws_index.end())
    {
        return false;
    }

    const auto it = _sessions.find(idx->second);
    if (it == _sessions.end())
    {
        // Index and sessions out of sync; drop the stale index entry.
        _ws_index.erase(idx);
        return false;
    }

    if (it->second.reference_count > 1)
    {
        it->second.reference_count--;
        _ws_index.erase(idx);
    }
    else
    {
        _ws_index.erase(idx);
        _sessions.erase(it);
    }
    return true;
}

bool UserSessionManager::is_online(const UserID& user_id) const
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _sessions.contains(user_id);
}

void UserSessionManager::display_sessions() const
{
    std::lock_guard<std::mutex> lock(_mtx);

    std::cout << "Users Count: " << _sessions.size() << std::endl;
    std::cout << "Users are: ";
    for (auto &session: _sessions)
    {
        std::cout << session.second.username << ':' << session.second.user_id << " Reference count: " << session.second.
                reference_count << ", " << std::endl;
    }
    std::cout << std::endl;
}
