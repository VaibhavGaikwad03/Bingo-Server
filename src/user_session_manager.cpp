#include "../include/user_session_manager.h"

UserSessionManager *UserSessionManager::_instance = nullptr;

UserSessionManager::UserSessionManager()
= default;

UserSessionManager::~UserSessionManager()
= default;

UserSessionManager *UserSessionManager::instance()
{
    if (_instance == nullptr)
    {
        _instance = new UserSessionManager();
        return _instance;
    }
    return _instance;
}

void UserSessionManager::destroy_instance()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void UserSessionManager::create_session(const UserID &user_id, const std::string &username,
                                    WebSocket *ws)
{
    if (!_sessions.contains(user_id)) // if not in map, create new session
    {
        UserSession session;
        session.user_id = user_id;
        session.username = username;
        session.ws = ws;
        session.reference_count = 1;

        _sessions[user_id] = session;
    }
    else // if already in map, just increment the reference count
    {
        UserSession *session = get_session(user_id);
        session->reference_count++;
    }
}

bool UserSessionManager::delete_session(UserSession *session)
{
    if ((session == nullptr) || (!_sessions.contains(session->user_id)))
    {
        return false;
    }

    if (session->reference_count > 1)
    {
        session->reference_count--;
    }
    else
    {
        _sessions.erase(session->user_id);
    }
    return true;
}

void UserSessionManager::display_sessions() const
{
    std::cout << "Users Count: " << _sessions.size() << std::endl;
    std::cout << "Users are: ";
    for (auto &session: _sessions)
    {
        std::cout << session.second.username << ':' << session.second.user_id << " Reference count: " << session.second.
                reference_count << ", " << std::endl;
    }
    std::cout << std::endl;
}

UserSession *UserSessionManager::get_session(const UserID user_id)
{
    if (!_sessions.contains(user_id))
        return nullptr;
    return &(_sessions[user_id]);
}

UserSession *UserSessionManager::get_session(const std::string &username)
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

UserSession *UserSessionManager::get_session(const WebSocket *ws)
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

bool UserSessionManager::is_session_exists(const UserID user_id)
{
    return (get_session(user_id) != nullptr);
}

bool UserSessionManager::is_session_exists(const std::string &username)
{
    return (get_session(username) != nullptr);
}

bool UserSessionManager::is_session_exists(const WebSocket *ws)
{
    return (get_session(ws) != nullptr);
}