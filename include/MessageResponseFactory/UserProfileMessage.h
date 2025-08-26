//
// Created by vaibz on 26/08/25.
//

#ifndef USERPROFILEMESSAGE_H
#define USERPROFILEMESSAGE_H

#include "IMessageResponse.h"

class UserProfileMessage final : IMessageResponse
{
    std::string _full_name;
    std::string _username;
    std::string _dob;
    std::string _gender;
    std::string _email;
    std::string _phone_number;

public:
    UserProfileMessage(std::string  full_name, std::string  username, std::string  dob, std::string  gender, std::string  email, std::string  phone_number);
    [[nodiscard]] nlohmann::json to_json() const override;
};

#endif //USERPROFILEMESSAGE_H