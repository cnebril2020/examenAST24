#ifndef USER_DATABASE_EXCEPTION_H
#define USER_DATABASE_EXCEPTION_H

#include <stdexcept>
#include <string>

/**
 * @brief Custom exception class for user database operations
 * 
 * This class provides specific error information for user database-related 
 * exceptions by extending std::runtime_error. It includes an error code 
 * system to better categorize different types of user database errors.
 */
class UserDatabaseException : public std::runtime_error {
public:
    /**
     * @brief Error codes for different types of user database errors
     */
    enum ErrorCode {
        UNKNOWN_ERROR = 0,
        FILE_ACCESS_ERROR = 1,    // File could not be opened/read/written
        DUPLICATE_USER = 2,       // User already exists in database
        USER_NOT_FOUND = 3,       // Requested user does not exist
        INVALID_DATA = 4,         // Invalid data provided (format, content)
        PERMISSION_DENIED = 5     // Operation not permitted (removing admin)
    };

    /**
     * @brief Constructor with message and error code
     * @param message Descriptive error message
     * @param code Specific error code from ErrorCode enum
     */
    UserDatabaseException(const std::string& message, 
                          ErrorCode code = UNKNOWN_ERROR);
    
    /**
     * @brief Get the error code
     * @return The ErrorCode enum value
     */
    ErrorCode getErrorCode() const;
    
    /**
     * @brief Get a string description of the error code
     * @return Human-readable error description
     */
    std::string getErrorString() const;

private:
    ErrorCode errorCode;
};

#endif // USER_DATABASE_EXCEPTION_H