#ifndef ERROR_CODES_H
#define ERROR_CODES_H

enum class ErrorCodes // generic error codes
{
    INVALID_USER_ID = -1
};

// specific error codes
enum class LoginErrorCodes
{
    NONE = 0,
    USERNAME_NOT_FOUND = 1001,
    PASSWORD_IS_INCORRECT = 1002,
    SOMETHING_WENT_WRONG = 1003,
};

enum class SignupErrorCodes
{
    NONE = 0,
    USERNAME_ALREADY_EXISTS = 2001,
    EMAIL_ALREADY_EXISTS = 2002,
    PHONE_ALREADY_EXISTS = 2003,
    SOMETHING_WENT_WRONG = 2004,
};

enum class ChangePasswordErrorCodes
{
    NONE = 0,
    NEW_PASSWORD_MUST_BE_DIFFERENT = 3001,
};

#endif //ERROR_CODES_H
