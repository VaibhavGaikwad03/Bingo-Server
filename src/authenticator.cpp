#include  "../include/log.h"
#include "../include/authenticator.h"

#include <uWebSockets/Utilities.h>

#include "../include/message_structures.h"
#include "../include/utils.h"

Authenticator::Authenticator()
{
    try
    {
        _session = std::make_unique<mysqlx::Session>("localhost", 33060, "vaibz", "p@ssw0rd", "bingo_server_db");
        _database = std::make_unique<mysqlx::Schema>(_session->getSchema("bingo_server_db"));
        _user_credentials_table = std::make_unique<mysqlx::Table>(_database->getTable("user_credentials"));
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, "", e.what());
    }
}

Authenticator::~Authenticator()
= default;

UserID Authenticator::login(const std::string &username, const std::string &password) const
// if successful returns user id else returns error code
{
    auto result = _user_credentials_table->select("user_id","username", "password")
    .where("username = :username")
    .bind("username", username)
    .execute();

    mysqlx::Row row = result.fetchOne();
    if (!row)
    {
        log(Log::ERROR, "", "Invalid username: " + username);
        return -1; // invalid username
    }

    std::string db_password = row[utils::to_underlying(UserCredentialsTableIndex::PASSWORD)].get<std::string>();
    if (db_password != password)
    {
        log(Log::ERROR, "", "Invalid password for user: " + username);
        return -2; // invalid password
    }

    log(Log::INFO, "", "User '" + username + "' logged in successfully");
    return row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)].get<UserID>();
}

UserID Authenticator::signup(const std::string& username, const std::string& password, const std::string& fullname, const std::string& gender, const std::string& dob, const std::string& email, const std::string& phone, const std::string& timestamp) // if successful returns user id else returns error code
{
    return -1;
}



