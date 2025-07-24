#include "../include/log.h"
#include "../include/utils.h"
#include "../include/error_codes.h"
#include "../include/message_handler.h"
#include "../include/message_parser.h"
#include "../include/message_structures.h"
#include "../include/session_manager.h"


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
    std::optional<LoginMessageRequest> parsed_message = MessageParser::login_message_request(message);
    if (!parsed_message.has_value())
    {
        return utils::to_underlying(LoginErrorCodes::SOMETHING_WENT_WRONG);
    }

    auto result = _user_credentials_table->select("user_id", "username", "password")
            .where("username = :username")
            .bind("username", parsed_message->username)
            .execute();

    mysqlx::Row row = result.fetchOne();
    if (!row) // fix it - compare db username and packet username
    {
        return utils::to_underlying(LoginErrorCodes::USERNAME_NOT_FOUND); // invalid username
    }

    auto db_password = row[utils::to_underlying(UserCredentialsTableIndex::PASSWORD)].get<std::string>();
    if (db_password != parsed_message->password)
    {
        return utils::to_underlying(LoginErrorCodes::PASSWORD_IS_INCORRECT); // invalid password
    }

    return row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)].get<UserID>();
}

Status MessageHandler::logout_request(const nlohmann::json &message) const
{
    std::optional<LogoutMessageRequest> parsed_message = MessageParser::logout_message_request(message);
    if (!parsed_message.has_value())
    {
        return Status::ERROR;
    }

    Session *session = SessionManager::instance()->get_session(parsed_message->user_id);
    if (session == nullptr)
    {
        return Status::ERROR;
    }

    SessionManager::instance()->delete_session(session);

    return Status::SUCCESS;
}

UserID MessageHandler::signup(const nlohmann::json &message) const
// if successful returns user id else returns error code
{
    std::optional<SignUpMessageRequest> parsed_message = MessageParser::signup_message_request(message);
    if (!parsed_message.has_value())
    {
        return utils::to_underlying(SignupErrorCodes::SOMETHING_WENT_WRONG);
    }

    // check if username exists
    auto result_username = _user_credentials_table->select("user_id")
            .where("username = :username")
            .bind("username", parsed_message->username)
            .execute();

    if (result_username.count() > 0)
    {
        return utils::to_underlying(SignupErrorCodes::USERNAME_ALREADY_EXISTS); // user already exists
    }

    // check if email exists
    auto result_email = _user_credentials_table->select("user_id")
            .where("email = :email")
            .bind("email", parsed_message->email)
            .execute();

    if (result_email.count() > 0)
    {
        return utils::to_underlying(SignupErrorCodes::EMAIL_ALREADY_EXISTS);
    }

    // check if phone exists
    auto result_phone = _user_credentials_table->select("user_id")
            .where("phone = :phone")
            .bind("phone", parsed_message->phone)
            .execute();

    if (result_phone.count() > 0)
    {
        return utils::to_underlying(SignupErrorCodes::PHONE_ALREADY_EXISTS);
    }

    auto insert_result = _user_credentials_table->insert("username", "password", "fullname", "gender", "dob", "email",
                                                         "phone"
                                                         /*, "signup_timestamp" client kadun ghyaycha ki mysql madhe auto generate karaycha??*/)
            .values(parsed_message->username, parsed_message->password, parsed_message->fullname,
                    parsed_message->gender,
                    parsed_message->dob, parsed_message->email, parsed_message->phone/*, timestamp*/)
            .execute();

    return static_cast<UserID>(insert_result.getAutoIncrementValue());
}

std::vector<FoundUser> MessageHandler::search_user(const nlohmann::json &message) const
{
    std::vector<FoundUser> users;

    std::optional<SearchUserRequest> parsed_request = MessageParser::search_user_request(message);
    if (!parsed_request.has_value())
    {
        return {};
    }

    if (parsed_request->username.empty())
        return {};

    try
    {
        mysqlx::RowResult result =
                _user_credentials_table
                ->select("*")
                .where("username LIKE :pattern AND username != :current_username")
                .bind("pattern", "%" + parsed_request->username + "%")
                .bind("current_username", parsed_request->requested_by) // Set this properly
                .execute();

        for (const mysqlx::Row &row: result)
        {
            FoundUser found_user;
            found_user.user_id = row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)];
            found_user.username = row[utils::to_underlying(UserCredentialsTableIndex::USERNAME)].get<std::string>();
            found_user.name = row[utils::to_underlying(UserCredentialsTableIndex::FULLNAME)].get<std::string>();

            mysqlx::Row request_status_result = _friend_request_table
                    ->select("request_status")
                    .where("sender = :sender AND receiver = :receiver")
                    .orderBy("timestamp DESC")
                    .limit(1)
                    .bind("sender", parsed_request->requested_by)
                    .bind("receiver", found_user.username)
                    .execute()
                    .fetchOne();

            if (request_status_result.isNull())
            {
                found_user.friendship_status = FriendshipStatus::NOT_FRIEND;
            }
            else
            {
                auto request_result = request_status_result[0].get<std::string>();
                std::cout << found_user.username << ": " << request_result << std::endl;

                if (request_result == "rejected")
                {
                    found_user.friendship_status = FriendshipStatus::NOT_FRIEND;
                }
                else if (request_result == "pending")
                {
                    found_user.friendship_status = FriendshipStatus::PENDING;
                }
                else
                {
                    found_user.friendship_status = FriendshipStatus::FRIEND;
                }
            }
            users.push_back(found_user);
        }
        std::cout << std::endl;
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, "", std::string("Database error in search_user: ") + err.what());
        return {};
    }
    catch (const std::exception &ex)
    {
        // Safety net for JSON / std errors
        log(Log::ERROR, "", std::string("Unexpected error in search_user: ") + ex.what());
        return {};
    }

    return users; // 5. All good → hand results to caller
}

bool MessageHandler::has_pending_friend_request(const int sender_id, const int receiver_id) const
{
    auto result = _friend_request_table->select("request_id")
            .where("sender_id = :sender_id AND receiver_id = :receiver_id AND request_status = 'pending'")
            .bind("sender_id", sender_id)
            .bind("receiver_id", receiver_id)
            .execute();

    if (result.count() > 0)
        return true;
    return false;
}

void MessageHandler::friend_req_request(const nlohmann::json &message) const
{
    std::optional<FriendReqRequest> parsed_request = MessageParser::friend_req_request(message);
    if (!parsed_request.has_value())
    {
        return;
    }

    try
    {
        if (has_pending_friend_request(parsed_request->sender_id, parsed_request->receiver_id))
            return; // already have pending friend request. entry nko karu jaa bhau parat.

        _friend_request_table->insert("sender_id", "sender", "name_of_sender", "receiver_id", "receiver",
                                      "name_of_receiver", "request_status")
                .values(parsed_request->sender_id, parsed_request->sender, parsed_request->name_of_sender,
                        parsed_request->receiver_id,
                        parsed_request->receiver, parsed_request->name_of_receiver, "pending")
                .execute();
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, "", std::string("Database error in MessageHandler::friend_req_request: ") + err.what());
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, "", std::string("Unexpected error in MessageHandler::friend_req_request: ") + ex.what());
    }
}

std::optional<UserProfile> MessageHandler::get_user_profile(const UserID user_id) const
{
    try
    {
        mysqlx::Row result = _user_credentials_table->select("username", "fullname", "CAST(dob AS CHAR)",
                                                             "gender", "email", "phone")
                .where("user_id = :user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result.isNull())
        {
            UserProfile user_profile;
            user_profile.username = result[0].get<std::string>();
            user_profile.name = result[1].get<std::string>();
            user_profile.dob = result[2].get<std::string>();
            user_profile.gender = result[3].get<std::string>();
            user_profile.email = result[4].get<std::string>();
            user_profile.phone = result[5].get<std::string>();

            return user_profile;
        }
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, "", std::string("Database error in MessageHandler::get_user_profile: ") + err.what());
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, "", std::string("Unexpected error in MessageHandler::get_user_profile: ") + ex.what());
    }

    return std::nullopt;
}

std::vector<Friend> MessageHandler::get_user_friends(const UserID user_id) const
{
    std::vector<Friend> friends;

    try
    {
        mysqlx::RowResult results = _friendship_table->select("friend_id", "friend", "friend_name")
                .where("user_id = :user_id")
                .bind("user_id", user_id)
                .execute();

        for (auto row: results)
        {
            Friend friend_obj;

            friend_obj.friend_id = row[0].get<int>();
            friend_obj.friend_username = row[1].get<std::string>();
            friend_obj.name_of_friend = row[2].get<std::string>();

            friends.push_back(friend_obj);
        }
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, "", std::string("Database error in MessageHandler::get_user_friends: ") + err.what());
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, "", std::string("Unexpected error in MessageHandler::get_user_friends: ") + ex.what());
    }

    return friends;
}

std::vector<PendingFriendRequest> MessageHandler::get_pending_friend_requests(const UserID user_id) const
{
    std::vector<PendingFriendRequest> pending_friend_requests;

    try
    {
        mysqlx::RowResult results = _friend_request_table->select("sender_id", "sender", "name_of_sender",
                                                                  "receiver_id",
                                                                  "receiver", "name_of_receiver", "request_status",
                                                                  "CAST(timestamp AS CHAR)")
                .where("receiver_id = :receiver_id AND request_status = 'pending'")
                .bind("receiver_id", user_id)
                .execute();

        for (auto row: results)
        {
            PendingFriendRequest pending_friend_request;

            pending_friend_request.sender_id = row[0].get<int>();
            pending_friend_request.sender = row[1].get<std::string>();
            pending_friend_request.name_of_sender = row[2].get<std::string>();
            pending_friend_request.receiver_id = row[3].get<int>();
            pending_friend_request.receiver = row[4].get<std::string>();
            pending_friend_request.name_of_receiver = row[5].get<std::string>();
            pending_friend_request.request_status = row[6].get<std::string>();
            pending_friend_request.timestamp = row[7].get<std::string>();

            pending_friend_requests.push_back(pending_friend_request);
        }
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, "",
            std::string("Database error in MessageHandler::get_pending_friend_requests: ") + err.what());
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, "",
            std::string("Unexpected error in MessageHandler::get_pending_friend_requests: ") + ex.what());
    }

    return pending_friend_requests;
}

std::vector<ChatMessage> MessageHandler::get_chat_messages(UserID user_id)
{
    return {};
}
