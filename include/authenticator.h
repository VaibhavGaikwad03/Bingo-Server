#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <mysqlx/xdevapi.h>

typedef int UserID;

class Authenticator
{
    std::unique_ptr<mysqlx::Session> _session;
    std::unique_ptr<mysqlx::Schema> _database;
    std::unique_ptr<mysqlx::Table> _user_credentials_table;

public:
    Authenticator();
    ~Authenticator();

    UserID login(const std::string &username, const std::string &password) const;
    UserID signup(const std::string& username, const std::string& password, const std::string& fullname, const std::string& gender, const std::string& dob, const std::string& email, const std::string& phone, const std::string& timestamp);
};

#endif //AUTHENTICATOR_H
