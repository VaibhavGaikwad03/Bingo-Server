//
// Created by vaibz on 26/08/25.
//

#include <utility>
#include "../../include/MessageResponseFactory/UserProfileMessage.h"
#include "../../include/message_keys.h"
#include "../../include/message_types.h"

UserProfileMessage::UserProfileMessage(std::string full_name, std::string username, std::string dob, std::string gender,
                                       std::string email, std::string phone_number) : _full_name(std::move(full_name)),
    _username(std::move(username)), _dob(std::move(dob)), _gender(std::move(gender)), _email(std::move(email)),
    _phone_number(std::move(phone_number))
{
}

nlohmann::json UserProfileMessage::to_json() const
{
    return {
        {MessageKeys::MESSAGE_TYPE, MessageType::USER_PROFILE_INFORMATION},
        {MessageKeys::FULLNAME, _full_name},
        {MessageKeys::USERNAME, _username},
        {MessageKeys::DOB, _dob},
        {MessageKeys::GENDER, _gender},
        {MessageKeys::EMAIL, _email},
        {MessageKeys::PHONE, _phone_number},
    };
}
