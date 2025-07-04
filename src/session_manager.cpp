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
    if (!_sessions.contains(user_id)) // if not in map, create new session
    {
        Session session;
        session.user_id = user_id;
        session.username = username;
        session.ws = ws;
        session.reference_count = 1;

        _sessions[user_id] = session;
    }
    else // if already in map, just increment the reference count
    {
        Session *session = get_session(user_id);
        session->reference_count++;
    }
}

void SessionManager::delete_session(const UserID user_id)
{
    Session *session = get_session(user_id);
    if (session->reference_count >= 1)
        session->reference_count--;
    else
        _sessions.erase(user_id);
}

void SessionManager::delete_session(const uWS::WebSocket<false, uWS::SERVER, std::string> *ws)
{
    for (auto it = _sessions.begin(); it != _sessions.end(); ++it)
    {
        std::cout << "ws1:" << it->second.ws << " ws2: " << ws << std::endl;
        if (it->second.ws == ws)
        {
            delete_session(it->second.user_id);
            break;
        }
    }
}

void SessionManager::display_sessions() const
{
    std::cout << "Count: " << _sessions.size() << std::endl;
    std::cout << "Users are: ";
    for (auto &session: _sessions)
    {
        std::cout << session.second.username << ':' << session.second.user_id << ": " << session.second.reference_count << ", " << std::endl;
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
