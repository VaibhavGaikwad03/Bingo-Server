#ifndef ERROR_CODES_H
#define ERROR_CODES_H

enum class LoginErrorCodes
{
    NONE = 0,
    USERNAME_NOT_FOUND = 1001,
    PASSWORD_IS_INCORRECT = 1002,
};

enum class SignupErrorCodes
{
    NONE = 0,
    USERNAME_ALREADY_EXISTS = 2001,
    EMAIL_ALREADY_EXISTS = 2002,
    PHONE_ALREADY_EXISTS = 2003,
};

#endif //ERROR_CODES_H
