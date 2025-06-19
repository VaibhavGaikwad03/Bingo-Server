#include  "../include/log.h"
#include "../include/authenticator.h"

#include <uWebSockets/Utilities.h>

#include "../include/error_codes.h"
#include "../include/message_structures.h"
#include "../include/utils.h"

Authenticator::Authenticator()
{
    try
    {
        _session = std::make_unique<mysqlx::Session>("localhost", 33060, "vaibz", "p@ssw0rd", "bingo_server_db");
        _database = std::make_unique<mysqlx::Schema>(_session->getSchema("bingo_server_db"));
        _user_credentials_table = std::make_unique<mysqlx::Table>(_database->getTable("user_credentials"));
    } catch (const std::exception &e)
    {
        log(Log::ERROR, "", e.what());
    }
}

Authenticator::~Authenticator()
= default;

UserID Authenticator::login(const std::string &username, const std::string &password) const
// if successful returns user id else returns error code
{
    auto result = _user_credentials_table->select("user_id", "username", "password")
            .where("username = :username")
            .bind("username", username)
            .execute();

    mysqlx::Row row = result.fetchOne();
    if (!row)
    {
        log(Log::ERROR, "", "Invalid username: " + username);
        return utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND); // invalid username
    }

    std::string db_password = row[utils::to_underlying(UserCredentialsTableIndex::PASSWORD)].get<std::string>();
    if (db_password != password)
    {
        log(Log::ERROR, "", "Invalid password for user: " + username);
        return utils::to_underlying(LoginErrorCodes::PASSWORD_IS_INCORRECT); // invalid password
    }

    log(Log::INFO, "", "User '" + username + "' logged in successfully");
    return row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)].get<UserID>();
}

UserID Authenticator::signup(const std::string &username, const std::string &password, const std::string &fullname,
                             const std::string &gender, const std::string &dob, const std::string &email,
                             const std::string &phone, const std::string &timestamp) const
// if successful returns user id else returns error code
{
    // Check if username exists
    auto result_username = _user_credentials_table->select("user_id")
            .where("username = :username")
            .bind("username", username)
            .execute();

    if (result_username.count() > 0)
    {
        log(Log::ERROR, "", "Username already exists: " + username);
        return utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS); // user already exists
    }

    // Check if email exists
    auto result_email = _user_credentials_table->select("user_id")
            .where("email = :email")
            .bind("email", email)
            .execute();

    if (result_email.count() > 0)
    {
        log(Log::ERROR, "", "Email already exists: " + email);
        return utils::to_underlying(SignupErrorCodes::EMAIL_ALREADY_EXISTS);
    }

    // Check if phone exists
    auto result_phone = _user_credentials_table->select("user_id")
            .where("phone = :phone")
            .bind("phone", phone)
            .execute();

    if (result_phone.count() > 0)
    {
        log(Log::ERROR, "", "Phone already exists: " + phone);
        return utils::to_underlying(SignupErrorCodes::PHONE_ALREADY_EXISTS);
    }

    auto insert_result = _user_credentials_table->insert("username", "password", "fullname", "gender", "dob", "email",
                                                         "phone"
                                                         /*, "signup_timestamp" client kadun ghyaycha ki mysql madhe auto generate karaycha??*/)
            .values(username, password, fullname, "Male" /*change it later*/, dob, email, phone/*, timestamp*/)
            .execute();

    return static_cast<UserID>(insert_result.getAutoIncrementValue());
}
