#ifndef USER_FACTORY_H
#define USER_FACTORY_H

#include "User.h"
#include "Admin.h"
#include "Employee.h"
#include <string>
#include <iostream>
#include <sstream>

// Structure representing a binary user record
struct UserRecord {
    u_int32_t userNumber;
    char nif[User::MAX_NIF];
    char pwd[User::MAX_STR];
    u_int32_t role;
};

class UserFactory {
public:
    // Binary record conversion methods
    static UserRecord userToRecord(const User* user);
    static User* recordToUser(const UserRecord& record);
    
    // Methods for direct binary file access
    static void writeUserToFile(std::ofstream& file, const User* user);
    static User* readUserFromFile(std::ifstream& file);
};

#endif // USER_FACTORY_H