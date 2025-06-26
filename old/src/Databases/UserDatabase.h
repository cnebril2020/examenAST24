#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <set>
#include <string>
#include "../Users/User.h"
#include "Database.h"

class User;

class UserDatabase : public Database {
public:
    UserDatabase(const char* filename = "users.dat");
    ~UserDatabase();

    // File Management (inherited from Database)
    bool loadFromFile(const char* filename) override;
    bool saveToFile(const char* filename) override;
    bool clearFile(const char* filename) override;
    
    // User Management
    User* login(u_int32_t userNumber, const char* NIF, const string& password);
    bool addUser(User* user);
    bool updateUser(User* user);
    bool removeUser(User* user);
    User* findUserByNumber(u_int32_t userNumber) const;
    bool changeUserPass(User* activeUser, User* targetUser);

    // Single getter 
    std::set<User*> getAllUsers() const;

private:
    std::set<User*> users;
};

#endif // USERDATABASE_H