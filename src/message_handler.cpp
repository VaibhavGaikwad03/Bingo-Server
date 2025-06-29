#include "../include/log.h"
#include "../include/utils.h"
#include "../include/error_codes.h"
#include "../include/message_handler.h"

#include "../include/message_parser.h"
#include "../include/message_structures.h"


MessageHandler::MessageHandler()
{
    try
    {
        _session = std::make_unique<mysqlx::Session>("localhost", 33060, "vaibz", "p@ssw0rd", "bingo_server_db");
        _database = std::make_unique<mysqlx::Schema>(_session->getSchema("bingo_server_db"));

        _user_credentials_table = std::make_unique<mysqlx::Table>(_database->getTable("user_credentials", true));
        _friendship_table = std::make_unique<mysqlx::Table>(_database->getTable("friendship"));
        _friend_request_table = std::make_unique<mysqlx::Table>(_database->getTable("friend_request"));
        _message_history_table = std::make_unique<mysqlx::Table>(_database->getTable("message_history"));
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, "", e.what());
        exit(EXIT_FAILURE);
    }
}

MessageHandler::~MessageHandler()
= default;


UserID MessageHandler::login(const nlohmann::json &message) const
// if successful returns user id else returns error code
{
    LoginMessageRequest parsed_message = MessageParser::login_message_request(message);

    auto result = _user_credentials_table->select("user_id", "username", "password")
            .where("username = :username")
            .bind("username", parsed_message.username)
            .execute();

    mysqlx::Row row = result.fetchOne();
    if (!row) // fix it - compare db username and packet username
    {
        return utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND); // invalid username
    }

    std::string db_password = row[utils::to_underlying(UserCredentialsTableIndex::PASSWORD)].get<std::string>();
    if (db_password != parsed_message.password)
    {
        return utils::to_underlying(LoginErrorCodes::PASSWORD_IS_INCORRECT); // invalid password
    }

    return row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)].get<UserID>();
}

UserID MessageHandler::signup(const nlohmann::json &message) const
// if successful returns user id else returns error code
{
    SignUpMessageRequest parsed_message = MessageParser::signup_message_request(message);

    // check if username exists
    auto result_username = _user_credentials_table->select("user_id")
            .where("username = :username")
            .bind("username", parsed_message.username)
            .execute();

    if (result_username.count() > 0)
    {
        return utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS); // user already exists
    }

    // check if email exists
    auto result_email = _user_credentials_table->select("user_id")
            .where("email = :email")
            .bind("email", parsed_message.email)
            .execute();

    if (result_email.count() > 0)
    {
        return utils::to_underlying(SignupErrorCodes::EMAIL_ALREADY_EXISTS);
    }

    // check if phone exists
    auto result_phone = _user_credentials_table->select("user_id")
            .where("phone = :phone")
            .bind("phone", parsed_message.phone)
            .execute();

    if (result_phone.count() > 0)
    {
        return utils::to_underlying(SignupErrorCodes::PHONE_ALREADY_EXISTS);
    }

    auto insert_result = _user_credentials_table->insert("username", "password", "fullname", "gender", "dob", "email",
                                                         "phone"
                                                         /*, "signup_timestamp" client kadun ghyaycha ki mysql madhe auto generate karaycha??*/)
            .values(parsed_message.username, parsed_message.password, parsed_message.fullname, parsed_message.gender,
                    parsed_message.dob, parsed_message.email, parsed_message.phone/*, timestamp*/)
            .execute();

    return static_cast<UserID>(insert_result.getAutoIncrementValue());
}

std::vector<FoundUser> MessageHandler::search_user(const nlohmann::json &message) const
{
    std::vector<FoundUser> users;
    const SearchUserRequest parsed_request = MessageParser::search_user_request(message);

    if (parsed_request.username.empty())
        return users;

    try
    {
        mysqlx::RowResult result =
                _user_credentials_table
                ->select("*")
                .where("username LIKE :pattern")
                .bind("pattern", "%" + parsed_request.username + "%") // "vai%" pattern
                .execute();

        for (const mysqlx::Row &row: result)
        {
            FoundUser found_user;
            found_user.user_id = row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)];
            found_user.username = row[utils::to_underlying(UserCredentialsTableIndex::USERNAME)].get<std::string>();
            found_user.name = row[utils::to_underlying(UserCredentialsTableIndex::FULLNAME)].get<std::string>();
            found_user.is_friend = Flag::NO; // change this later
            users.push_back(found_user);
            //std::cout << "search result: " << found_user.username << std::endl;
        }
        std::cout << std::endl;
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, "", std::string("Database error in search_user: ") + err.what());
        throw; // or return {};  ↩ choose your policy
    }
    catch (const std::exception &ex)
    {
        // Safety net for JSON / std errors
        log(Log::ERROR, "", std::string("Unexpected error in search_user: ") + ex.what());
        throw;
    }

    return users; // 5. All good → hand results to caller
}
