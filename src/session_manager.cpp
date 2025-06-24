#include "../include/session_manager.h"

SessionManager *SessionManager::_instance = nullptr;

SessionManager::SessionManager()
= default;

SessionManager::~SessionManager()
= default;

SessionManager *SessionManager::instance()
{
    if (_instance == nullptr)
    {
        _instance = new SessionManager();
        return _instance;
    }
    return _instance;
}

void SessionManager::destroy_instance()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void SessionManager::create_session(const UserID &user_id, const std::string &username,
                                    uWS::WebSocket<false, uWS::SERVER, std::string> *ws)
{
    Session session;
    session.user_id = user_id;
    session.username = username;
    session.ws = ws;

    _sessions[user_id] = session;
}

void SessionManager::delete_session(const UserID user_id)
{
    _sessions.erase(user_id);
}

void SessionManager::delete_session(const uWS::WebSocket<false, uWS::SERVER, std::string> *ws)
{
    for (auto it = _sessions.begin(); it != _sessions.end(); ++it)
    {
        if (it->second.ws == ws)
        {
            _sessions.erase(it);
            break;
        }
    }
}

void SessionManager::display_sessions() const
{
    std::cout << "Users are: ";
    for (auto &session: _sessions)
    {
        std::cout << session.second.username << ':' << session.second.user_id << ", ";
    }
    std::cout << std::endl;
}

Session *SessionManager::get_session(const UserID user_id)
{
    if (!_sessions.contains(user_id))
        return nullptr;
    return &(_sessions[user_id]);
}

Session *SessionManager::get_session(const std::string &username)
{
    for (auto &session: _sessions)
    {
        if (session.second.username == username)
        {
            return &(session.second);
        }
    }
    return nullptr;
}

Session *SessionManager::get_session(uWS::WebSocket<false, uWS::SERVER, std::string> *ws)
{
    for (auto &session: _sessions)
    {
        if (session.second.ws == ws)
        {
            return &(session.second);
        }
    }
    return nullptr;
}
