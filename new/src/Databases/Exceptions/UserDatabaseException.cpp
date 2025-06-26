#include "UserDatabaseException.h"

UserDatabaseException::UserDatabaseException(const std::string& message, 
                                             ErrorCode code)
    : std::runtime_error(message), errorCode(code) {}

UserDatabaseException::ErrorCode UserDatabaseException::getErrorCode() const {
    return errorCode;
}

std::string UserDatabaseException::getErrorString() const {
    switch (errorCode) {
        case FILE_ACCESS_ERROR: return "File Access Error";
        case DUPLICATE_USER: return "Duplicate User";
        case USER_NOT_FOUND: return "User Not Found";
        case INVALID_DATA: return "Invalid Data";
        case PERMISSION_DENIED: return "Permission Denied";
        default: return "Unknown Error";
    }
}