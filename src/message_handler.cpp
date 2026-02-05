#include "../include/utils/logger.h"
#include "../include/utils/utils.h"
#include "../include/error_codes.h"
#include "../include/message_handler.h"
#include "../include/message_parser.h"
#include "../include/message_structures.h"
#include "../include/user_session_manager.h"
#include "../include/utils/token_generator.h"

MessageHandler::MessageHandler(const DatabaseConfig& db_config)
{
    try
    {
        _session = std::make_unique<mysqlx::Session>(
            db_config.host, db_config.port, 
            db_config.username, db_config.password, 
            db_config.name);
        _database = std::make_unique<mysqlx::Schema>(_session->getSchema(db_config.name));

        _user_credentials_table = std::make_unique<mysqlx::Table>(_database->getTable("user_credentials", true));
        _friendship_table = std::make_unique<mysqlx::Table>(_database->getTable("friendship"));
        _friend_request_table = std::make_unique<mysqlx::Table>(_database->getTable("friend_request"));
        _auth_tokens_table = std::make_unique<mysqlx::Table>(_database->getTable("auth_tokens"));
        _chat_history_table = std::make_unique<mysqlx::Table>(_database->getTable("chat_history"));
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        exit(EXIT_FAILURE);
    }
}

MessageHandler::~MessageHandler()
= default;

// getters
std::string MessageHandler::get_username(const UserID user_id) const
{
    try
    {
        auto result = _user_credentials_table->select("username")
                .where("user_id=:user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result)
            return "";

        return result[0].get<std::string>();
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return "";
    }
}

std::string MessageHandler::get_fullname(const UserID user_id) const
{
    try
    {
        auto result = _user_credentials_table->select("fullname")
                .where("user_id=:user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result)
            return "";

        return result[0].get<std::string>();
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return "";
    }
}

std::string MessageHandler::get_dob(const UserID user_id) const
{
    try
    {
        auto result = _user_credentials_table->select("dob")
                .where("user_id=:user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result)
            return "";

        return result[0].get<std::string>();
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return "";
    }
}

std::string MessageHandler::get_gender(const UserID user_id) const
{
    try
    {
        auto result = _user_credentials_table->select("gender")
                .where("user_id=:user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result)
            return "";

        return result[0].get<std::string>();
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return "";
    }
}

std::string MessageHandler::get_email(const UserID user_id) const
{
    try
    {
        auto result = _user_credentials_table->select("email")
                .where("user_id=:user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result)
            return "";

        return result[0].get<std::string>();
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return "";
    }
}

std::string MessageHandler::get_phone(const UserID user_id) const
{
    try
    {
        auto result = _user_credentials_table->select("phone")
                .where("user_id=:user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result)
            return "";

        return result[0].get<std::string>();
    }
    catch (const std::exception &e)
    {
        log(Log::ERROR, __func__, e.what());
        return "";
    }
}

std::optional<LoginMessageResponse> MessageHandler::login_request(const nlohmann::json &message) const
// if successful returns user id else returns error code
{
    std::optional<LoginMessageRequest> parsed_message = MessageParser::login_message_request(message);
    if (!parsed_message.has_value())
    {
        return std::nullopt;
    }

    auto result = _user_credentials_table->select("user_id", "username", "password")
            .where("username = :username")
            .bind("username", parsed_message->username)
            .execute();

    mysqlx::Row row = result.fetchOne();
    if (!row) // fix it - compare db username and packet username
    {
        LoginMessageResponse login_response(Status::ERROR, -1, LoginErrorCode::USERNAME_NOT_FOUND, "");
        return login_response; // invalid username
    }

    auto db_password = row[utils::to_underlying(UserCredentialsTableIndex::PASSWORD)].get<std::string>();
    if (db_password != parsed_message->password)
    {
        LoginMessageResponse login_response(Status::ERROR, -1, LoginErrorCode::PASSWORD_IS_INCORRECT, "");
        return login_response; // invalid password
    }

    UserID valid_user_id = row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)].get<UserID>();

    auto row_token = _auth_tokens_table->select("token")
            .where("user_id = :user_id")
            .bind("user_id", valid_user_id)
            .execute()
            .fetchOne();

    std::string new_token;
    if (!row_token)
    {
        new_token = generate_token(32);
        _auth_tokens_table->insert("user_id", "token")
                .values(valid_user_id, new_token)
                .execute();
    }

    if (new_token.empty())
    {
        new_token = row_token[0].get<std::string>();
    }

    LoginMessageResponse login_response(Status::SUCCESS, valid_user_id, LoginErrorCode::NONE, new_token);
    return login_response;
}

std::optional<LogoutMessageResponse> MessageHandler::logout_request(const nlohmann::json &message) const
{
    const std::optional<LogoutMessageRequest> parsed_message = MessageParser::logout_message_request(message);
    if (!parsed_message.has_value())
    {
        LogoutMessageResponse logout_message_response(Status::ERROR);
        return logout_message_response;
    }

    UserSession *session = UserSessionManager::instance().get_session(parsed_message->user_id);
    if (session == nullptr)
    {
        LogoutMessageResponse logout_message_response(Status::ERROR);
        return logout_message_response;
    }

    LogoutMessageResponse logout_message_response(Status::SUCCESS);
    return logout_message_response;
}

std::optional<SignupMessageResponse> MessageHandler::signup_request(const nlohmann::json &message) const
// if successful returns user id else returns error code
{
    const std::optional<SignUpMessageRequest> parsed_message = MessageParser::signup_message_request(message);
    if (!parsed_message.has_value())
    {
        SignupMessageResponse signup_message_response(Status::ERROR, static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                      SignupErrorCode::SOMETHING_WENT_WRONG);
        return signup_message_response;
        // return utils::to_underlying(SignupErrorCode::SOMETHING_WENT_WRONG);
    }

    // check if username exists
    auto result_username = _user_credentials_table->select("user_id")
            .where("username = :username")
            .bind("username", parsed_message->username)
            .execute();

    if (result_username.count() > 0)
    {
        SignupMessageResponse signup_message_response(Status::ERROR, static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                      SignupErrorCode::USERNAME_ALREADY_EXISTS);
        return signup_message_response;
        // return utils::to_underlying(SignupErrorCode::USERNAME_ALREADY_EXISTS); // user already exists
    }

    // check if email exists
    auto result_email = _user_credentials_table->select("user_id")
            .where("email = :email")
            .bind("email", parsed_message->email)
            .execute();

    if (result_email.count() > 0)
    {
        SignupMessageResponse signup_message_response(Status::ERROR, static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                      SignupErrorCode::EMAIL_ALREADY_EXISTS);
        return signup_message_response;
        // return utils::to_underlying(SignupErrorCode::EMAIL_ALREADY_EXISTS);
    }

    // check if phone exists
    auto result_phone = _user_credentials_table->select("user_id")
            .where("phone = :phone")
            .bind("phone", parsed_message->phone)
            .execute();

    if (result_phone.count() > 0)
    {
        SignupMessageResponse signup_message_response(Status::ERROR, static_cast<UserID>(ErrorCode::INVALID_USER_ID),
                                                      SignupErrorCode::PHONE_ALREADY_EXISTS);
        return signup_message_response;
        // return utils::to_underlying(SignupErrorCode::PHONE_ALREADY_EXISTS);
    }

    const auto insert_result = _user_credentials_table->insert("username", "password", "fullname", "gender", "dob", "email",
                                                         "phone"
                                                         /*, "signup_timestamp" client kadun ghyaycha ki mysql madhe auto generate karaycha??*/)
            .values(parsed_message->username, parsed_message->password, parsed_message->fullname,
                    parsed_message->gender,
                    parsed_message->dob, parsed_message->email, parsed_message->phone/*, timestamp*/)
            .execute();

    SignupMessageResponse signup_message_response(Status::SUCCESS,
                                                  static_cast<UserID>(insert_result.getAutoIncrementValue()),
                                                  SignupErrorCode::NONE);
    return signup_message_response;
    // return static_cast<UserID>(insert_result.getAutoIncrementValue());
}

std::optional<SearchUserRequestMessageResponse> MessageHandler::search_user_request(const nlohmann::json &message) const
{
    const std::optional<SearchUserRequest> parsed_request = MessageParser::search_user_request(message);
    if (!parsed_request.has_value() || parsed_request->username.empty())
    {
        SearchUserRequestMessageResponse search_user_request_message_response(0, {});
        return search_user_request_message_response;
    }

    // if (parsed_request->username.empty())
    //     return {};

    try
    {
        std::vector<FoundUser> users;

        mysqlx::RowResult result =
                _user_credentials_table
                ->select("*")
                .where(
                    "(username LIKE :username_pattern OR fullname LIKE :fullname_pattern) AND username != :current_username")
                .limit(30)
                .bind("username_pattern", parsed_request->username + "%")
                .bind("fullname_pattern",  "%" + parsed_request->username + "%")
                .bind("current_username", parsed_request->requested_by) // Set this properly
                .execute();

        for (const mysqlx::Row &row: result)
        {
            FoundUser found_user;
            found_user.user_id = row[utils::to_underlying(UserCredentialsTableIndex::USER_ID)];
            found_user.username = row[utils::to_underlying(UserCredentialsTableIndex::USERNAME)].get<std::string>();
            found_user.display_name = row[utils::to_underlying(UserCredentialsTableIndex::FULLNAME)].get<std::string>();

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
                found_user.friendship_status = FriendshipState::NOT_FRIEND;
            }
            else
            {
                auto request_result = request_status_result[0].get<std::string>();

                if (request_result == "rejected")
                {
                    found_user.friendship_status = FriendshipState::NOT_FRIEND;
                }
                else if (request_result == "pending")
                {
                    found_user.friendship_status = FriendshipState::PENDING;
                }
                else
                {
                    found_user.friendship_status = FriendshipState::FRIEND;
                }
            }
            users.push_back(found_user); // 5. All good → hand results to caller
        }

        SearchUserRequestMessageResponse search_user_request_message_response(users.size(), users);
        return search_user_request_message_response;
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__, std::string("Database error in search_user: ") + err.what());
        SearchUserRequestMessageResponse search_user_request_message_response(0, {});
        return search_user_request_message_response;
    }
    catch (const std::exception &ex)
    {
        // Safety net for JSON / std errors
        log(Log::ERROR, __func__, std::string("Unexpected error in search_user: ") + ex.what());
        SearchUserRequestMessageResponse search_user_request_message_response(0, {});
        return search_user_request_message_response;
    }
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
    const std::optional<FriendReqRequest> parsed_request = MessageParser::friend_req_request(message);
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

void MessageHandler::friend_req_response(const nlohmann::json &message) const
{
    std::optional<FriendReqResponse> parsed_request = MessageParser::friend_req_response(message);
    if (!parsed_request.has_value())
    {
        return;
    }

    try
    {
        std::string request_status;

        if (parsed_request->status == FriendRequestState::ACCEPTED)
        {
            request_status = "accepted";
        }
        else
        {
            request_status = "rejected";
        }

        _friend_request_table->update()
                .set("request_status", request_status)
                .where("sender_id = :sender_id AND receiver_id = :receiver_id")
                .bind("sender_id", parsed_request->receiver_id)
                .bind("receiver_id", parsed_request->sender_id)
                .execute();

        auto result = _friend_request_table->select("request_id")
                .where("sender_id = :sender_id AND receiver_id = :receiver_id")
                .bind("sender_id", parsed_request->receiver_id)
                .bind("receiver_id", parsed_request->sender_id)
                .execute()
                .fetchOne();

        int request_id = -1;
        if (!result.isNull())
        {
            request_id = result[0].get<int>();
        }

        if ((parsed_request->status == FriendRequestState::ACCEPTED) && (request_id != -1))
        {
            _friendship_table->insert("friendship_id", "user_id", "user", "name_of_user",
                                      "friend_id", "friend", "name_of_friend")
                    .values(request_id, parsed_request->sender_id, parsed_request->sender,
                            parsed_request->name_of_sender, parsed_request->receiver_id, parsed_request->receiver,
                            parsed_request->name_of_receiver)
                    .execute();

            _friendship_table->insert("friendship_id", "user_id", "user", "name_of_user",
                                      "friend_id", "friend", "name_of_friend")
                    .values(request_id, parsed_request->receiver_id, parsed_request->receiver,
                            parsed_request->name_of_receiver, parsed_request->sender_id, parsed_request->sender,
                            parsed_request->name_of_sender)
                    .execute();
        }
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, "", std::string("Database error in MessageHandler::friend_req_response: ") + err.what());
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, "", std::string("Unexpected error in MessageHandler::friend_req_response: ") + ex.what());
    }
}


std::optional<UserProfileMessage> MessageHandler::get_user_profile(const UserID user_id) const
{
    try
    {
        mysqlx::Row result = _user_credentials_table->select("fullname", "username", "CAST(dob AS CHAR)",
                                                             "gender", "email", "phone")
                .where("user_id = :user_id")
                .bind("user_id", user_id)
                .execute()
                .fetchOne();

        if (!result.isNull())
        {
            UserProfileMessage user_profile_message(result[0].get<std::string>(), result[1].get<std::string>(),
                                                    result[2].get<std::string>(), result[3].get<std::string>(),
                                                    result[4].get<std::string>(), result[5].get<std::string>());
            return user_profile_message;
        }
        else
        {
            return std::nullopt;
        }
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__, std::string("Database error: ") + err.what());
        return std::nullopt;
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__, std::string("Unexpected error: ") + ex.what());
        return std::nullopt;
    }
}

std::optional<FriendsListMessage> MessageHandler::get_user_friends(const UserID user_id) const
{
    try
    {
        std::vector<FriendInfo> friends_list;

        auto friendship_query_result = _friendship_table->select("friend_id", "friend", "name_of_friend")
                .where("user_id = :user_id")
                .bind("user_id", user_id)
                .execute();

        for (auto friendship_row: friendship_query_result)
        {
            auto user_profile_row = _user_credentials_table->select("gender", "CAST(dob AS CHAR)")
                    .where("user_id = :user_id")
                    .bind("user_id", friendship_row[0].get<UserID>())
                    .execute()
                    .fetchOne();

            FriendInfo friend_info;

            friend_info.user_id = friendship_row[0].get<UserID>();
            friend_info.username = friendship_row[1].get<std::string>();
            friend_info.full_name = friendship_row[2].get<std::string>();
            friend_info.gender = user_profile_row[0].get<std::string>();
            friend_info.dob = user_profile_row[1].get<std::string>();

            friends_list.push_back(friend_info);
        }

        FriendsListMessage friends_list_response_message(friends_list.size(), friends_list);

        return friends_list_response_message;
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__, std::string("Database error: ") + err.what());
        return std::nullopt;
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__, std::string("Unexpected error: ") + ex.what());
        return std::nullopt;
    }
}

std::optional<PendingFriendRequests> MessageHandler::get_pending_friend_requests(const UserID user_id) const
{
    try
    {
        std::vector<PendingFriendRequestInfo> pending_friend_requests;

        mysqlx::RowResult results = _friend_request_table->select("sender_id", "sender", "name_of_sender",
                                                                  "receiver_id",
                                                                  "receiver", "name_of_receiver", "request_status",
                                                                  "CAST(timestamp AS CHAR)")
                .where("receiver_id = :receiver_id AND request_status = 'pending'")
                .bind("receiver_id", user_id)
                .execute();

        for (auto row: results)
        {
            PendingFriendRequestInfo pending_friend_request_info;

            pending_friend_request_info.sender_id = row[0].get<int>();
            pending_friend_request_info.sender = row[1].get<std::string>();
            pending_friend_request_info.name_of_sender = row[2].get<std::string>();
            pending_friend_request_info.receiver_id = row[3].get<int>();
            pending_friend_request_info.receiver = row[4].get<std::string>();
            pending_friend_request_info.name_of_receiver = row[5].get<std::string>();
            pending_friend_request_info.request_status = row[6].get<std::string>();
            pending_friend_request_info.timestamp = row[7].get<std::string>();

            pending_friend_requests.push_back(pending_friend_request_info);
        }

        PendingFriendRequests pending_friend_requests_response_message(pending_friend_requests.size(),
                                                                       pending_friend_requests);
        return pending_friend_requests_response_message;
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__, std::string("Database error: ") + err.what());
        return std::nullopt;
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__, std::string("Unexpected error: ") + ex.what());
        return std::nullopt;
    }
}

// std::vector<PendingFriendRequest> MessageHandler::get_pending_friend_requests(const UserID user_id) const
// {
//     std::vector<PendingFriendRequest> pending_friend_requests;
//
//     try
//     {
//         mysqlx::RowResult results = _friend_request_table->select("sender_id", "sender", "name_of_sender",
//                                                                   "receiver_id",
//                                                                   "receiver", "name_of_receiver", "request_status",
//                                                                   "CAST(timestamp AS CHAR)")
//                 .where("receiver_id = :receiver_id AND request_status = 'pending'")
//                 .bind("receiver_id", user_id)
//                 .execute();
//
//         for (auto row: results)
//         {
//             PendingFriendRequest pending_friend_request;
//
//             pending_friend_request.sender_id = row[0].get<int>();
//             pending_friend_request.sender = row[1].get<std::string>();
//             pending_friend_request.name_of_sender = row[2].get<std::string>();
//             pending_friend_request.receiver_id = row[3].get<int>();
//             pending_friend_request.receiver = row[4].get<std::string>();
//             pending_friend_request.name_of_receiver = row[5].get<std::string>();
//             pending_friend_request.request_status = row[6].get<std::string>();
//             pending_friend_request.timestamp = row[7].get<std::string>();
//
//             pending_friend_requests.push_back(pending_friend_request);
//         }
//     }
//     catch (const mysqlx::Error &err)
//     {
//         log(Log::ERROR, "",
//             std::string("Database error in MessageHandler::get_pending_friend_requests: ") + err.what());
//     }
//     catch (const std::exception &ex)
//     {
//         log(Log::ERROR, "",
//             std::string("Unexpected error in MessageHandler::get_pending_friend_requests: ") + ex.what());
//     }
//
//     return pending_friend_requests;
// }

std::vector<ChatMessage> MessageHandler::get_chat_messages(UserID user_id)
{
    return {};
}

ChangePasswordErrorCode MessageHandler::change_password_request(const nlohmann::json &message) const
{
    std::optional<ChangePassword> parsed_request = MessageParser::change_password_request(message);
    if (!parsed_request.has_value())
    {
        return ChangePasswordErrorCode::SOMETHING_WENT_WRONG;
    }

    try
    {
        auto result = _user_credentials_table->select("password")
                .where("user_id = :user_id")
                .bind("user_id", parsed_request->user_id)
                .execute()
                .fetchOne();

        if (result)
        {
            const auto db_password = result[0].get<std::string>();

            if (parsed_request->new_password != db_password)
            {
                _user_credentials_table->update()
                        .set("password", parsed_request->new_password)
                        .where("user_id = :user_id")
                        .bind("user_id", parsed_request->user_id)
                        .execute();

                return ChangePasswordErrorCode::NONE;
            }
            else
            {
                return ChangePasswordErrorCode::NEW_PASSWORD_MUST_BE_DIFFERENT;
            }
        }
        else
        {
            return ChangePasswordErrorCode::SOMETHING_WENT_WRONG;
        }
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__,
            std::string("Database error: ") + err.what());
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__,
            std::string("Unexpected error: ") + ex.what());
    }
    return ChangePasswordErrorCode::SOMETHING_WENT_WRONG;
}

std::optional<ReconnectResponse> MessageHandler::reconnect_request(const nlohmann::json &message) const
{
    try
    {
        const std::optional<ReconnectRequest> parsed_request = MessageParser::reconnect_request(message);
        if (!parsed_request.has_value())
        {
            ReconnectResponse reconnect_response(Status::ERROR, -1, "", ReconnectErrorCode::SOMETHING_WENT_WRONG);
            return reconnect_response;
        }

        auto result = _auth_tokens_table->select("token", "user_id")
                .where("user_id = :user_id")
                .bind("user_id", parsed_request->user_id)
                .execute()
                .fetchOne();

        if (result)
        {
            const std::string db_auth_token = result[0].get<std::string>();
            if (parsed_request->auth_token != db_auth_token)
            {
                ReconnectResponse reconnect_response(Status::ERROR, -1, "", ReconnectErrorCode::INVALID_AUTH_TOKEN);
                return reconnect_response;
            }

            ReconnectResponse reconnect_response(Status::SUCCESS, result[1].get<UserID>(), db_auth_token, ReconnectErrorCode::NONE);
            return reconnect_response;
        }

        ReconnectResponse reconnect_response(Status::ERROR, -1, "", ReconnectErrorCode::INVALID_AUTH_TOKEN);
        return reconnect_response;
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__,
            std::string("Database error: ") + err.what());
        return std::nullopt;
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__,
            std::string("Unexpected error: ") + ex.what());
        return std::nullopt;
    }
}

std::optional<UpdateProfileResponse> MessageHandler::update_profile_request(const nlohmann::json &message) const
{
    try
    {
        const std::optional<UpdateProfileRequest> parsed_request = MessageParser::update_profile_request(message);
        if (!parsed_request.has_value())
        {
            UpdateProfileResponse update_profile_response(Status::ERROR, UpdateProfileErrorCode::SOMETHING_WENT_WRONG);
            return update_profile_response;
        }

        const auto result = _user_credentials_table->select("username", "fullname", "dob", "gender", "email", "phone")
                .where("user_id = :user_id")
                .bind("user_id", parsed_request->user_id)
                .execute()
                .fetchOne();

        if (result)
        {
            bool is_details_changed = false;
            mysqlx::TableUpdate update = _user_credentials_table->update()
                    .where("user_id = :user_id")
                    .bind("user_id", parsed_request->user_id);

            if (result[0].get<std::string>() != parsed_request->username)
            {
                update.set("username", parsed_request->username);
                is_details_changed = true;
            }

            if (result[1].get<std::string>() != parsed_request->fullname)
            {
                update.set("fullname", parsed_request->fullname);
                is_details_changed = true;
            }

            if (result[2].get<std::string>() != parsed_request->dob)
            {
                update.set("dob", parsed_request->dob);
                is_details_changed = true;
            }

            if (result[3].get<std::string>() != parsed_request->gender)
            {
                update.set("gender", parsed_request->gender);
                is_details_changed = true;
            }

            if (result[4].get<std::string>() != parsed_request->email)
            {
                update.set("email", parsed_request->email);
                is_details_changed = true;
            }

            if (result[5].get<std::string>() != parsed_request->phone)
            {
                update.set("phone", parsed_request->phone);
                is_details_changed = true;
            }

            if (is_details_changed)
            {
                update.execute();
            }

            UpdateProfileResponse update_profile_response(Status::SUCCESS, UpdateProfileErrorCode::NONE);
            return update_profile_response;
        }
        else
        {
            UpdateProfileResponse update_profile_response(Status::ERROR, UpdateProfileErrorCode::SOMETHING_WENT_WRONG);
            return update_profile_response;
        }
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__,
            std::string("Database error: ") + err.what());
        return std::nullopt;
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__,
            std::string("Unexpected error: ") + ex.what());
        return std::nullopt;
    }
}

void MessageHandler::chat_message(const nlohmann::json &message) const
{
    try
    {

    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__,
            std::string("Database error: ") + err.what());
        return;
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__,
            std::string("Unexpected error: ") + ex.what());
        return;
    }
}

MessageID MessageHandler::get_message_id_request() const
{
    try
    {
        _session->sql(
            "UPDATE message_id_counter "
                    "SET value = LAST_INSERT_ID(value + 1) "
                    "WHERE id = 1").execute();

        mysqlx::RowResult result = static_cast<mysqlx::RowResult>(_session->sql("SELECT LAST_INSERT_ID()").execute());
        mysqlx::Row row = result.fetchOne();

        if (!row)
        {
            log(Log::ERROR, __func__, "Failed to fetch new message ID");
            return -1;
        }

        return row[0].get<MessageID>();
    }
    catch (const mysqlx::Error &err)
    {
        log(Log::ERROR, __func__,
            std::string("Database error: ") + err.what());
        return -1;
    }
    catch (const std::exception &ex)
    {
        log(Log::ERROR, __func__,
            std::string("Unexpected error: ") + ex.what());
        return -1;
    }
}
