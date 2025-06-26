#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "UserDatabase.h"
#include "../Users/User.h"
#include "../Users/Admin.h"
#include "../Users/Employee.h"
#include "../Users/UserFactory.h"
#include "Exceptions/UserDatabaseException.h"

class User;
class Admin;
class Employee;
class UserFactory;

using namespace std;

// Constructor
UserDatabase::UserDatabase(const char* inputFilename) {
    // Copy the input filename to the member variable with bounds checking
    strncpy(this->filename, inputFilename, MAX_STR - 1);
    this->filename[MAX_STR - 1] = '\0'; // Ensure null termination
    
    // Make sure the filename has .dat extension
    size_t len = strlen(this->filename);
    if (len < 4 || strcmp(this->filename + len - 4, ".dat") != 0) {
        // If filename doesn't end with .dat, append it (if there's space)
        if (len + 4 < MAX_STR) {
            strcat(this->filename, ".dat");
        } else {
            cerr << "Warning: Filename too long to append .dat extension\n";
        }
    }

    // First, load existing users from file if it exists
    loadFromFile(this->filename);

    // After loading, check if default admin exists
    u_int32_t adminNumber = 10000;
    const char* NIF = "00000000";
    const char* pwd = "admin";

    // Only create admin if doesn't exists
    if (findUserByNumber(adminNumber) == nullptr) {
        Admin* defaultAdmin = new Admin(adminNumber, NIF, pwd, User::ADMIN);
        this->users.insert(defaultAdmin);
    }
}

// Destructor
UserDatabase::~UserDatabase() {
    // Save to file before destroying
    saveToFile(this->filename);
    
    // Free memory for all users
    for (auto user : users) {
        delete user;
    }
}

bool UserDatabase::loadFromFile(const char* filename) {
    // Open file in binary mode
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Warning: Could not open file '" 
            << filename << "' for reading." << endl;
        return false;
    }

    // Read records until end of file
    User* user = nullptr;
    while ((user = UserFactory::readUserFromFile(file)) != nullptr) {
        // Try to add the user (set will handle duplicates)
        if (!users.insert(user).second) {
            cerr << "Warning: Duplicate user with number " 
                 << user->getuserNumber() << " ignored" << endl;
            delete user; // Clean up if not inserted
        }
    }

    file.close();
    return true;
}

bool UserDatabase::saveToFile(const char* filename) {
    // Open file in binary mode with truncation
    ofstream file(filename, ios::out | ios::binary | ios::trunc);
    if (!file.is_open()) {
        throw runtime_error("Could not open file for writing.");
    }

    // Write each user as a binary record
    for (auto user : users) {
        UserFactory::writeUserToFile(file, user);
    }

    file.close();
    return true;
}

bool UserDatabase::clearFile(const char* filename) {
    // First, identify and preserve the default admin user (10000)
    User* defaultAdmin = nullptr;
    for (auto user : users) {
        if (user->getuserNumber() == 10000) {
            defaultAdmin = user;
            break;
        }
    }
    
    // Delete all users except default admin
    for (auto it = users.begin(); it != users.end();) {
        if (*it != defaultAdmin) {
            delete *it;  // Free memory
            it = users.erase(it);  // Remove from set and get next iterator
        } else {
            ++it;  // Skip default admin
        }
    }
    
    // Open file in binary mode with truncation
    ofstream file(filename, ios::out | ios::binary | ios::trunc);
    if (!file.is_open()) {
        throw runtime_error("Could not open file for clearing.");
    }
    
    // Write the default admin back to the file if it exists
    if (defaultAdmin) {
        UserFactory::writeUserToFile(file, defaultAdmin);
    }
    
    file.close();
    return true;
}

User* UserDatabase::login(u_int32_t userNumber, 
                          const char* NIF, 
                          const string& password) {
    User* user = findUserByNumber(userNumber);
    
    if (!user) {
        return nullptr; // User not found
    }
    
    // Check if NIF and password match
    if (strcmp(user->getNif(), NIF) == 0 && 
        strcmp(user->getPwd().c_str(), password.c_str()) == 0) {
        return user;
    }
    
    return nullptr; // Authentication failed
}

bool UserDatabase::addUser(User* user) {
    if (!user) {
        throw invalid_argument("Cannot add null user");
    }
    
    // Check if a user with this number already exists
    if (findUserByNumber(user->getuserNumber()) != nullptr) {
        return false; // User already exists
    }
    
    // Validate user number range
    if (user->getuserNumber() < User::MIN_USER_NUMBER || 
        user->getuserNumber() > User::MAX_USER_NUMBER) {
        throw invalid_argument("User number must be between " + 
                              to_string(User::MIN_USER_NUMBER) + " and " + 
                              to_string(User::MAX_USER_NUMBER));
    }
    
    // Insert the user into the set
    auto result = users.insert(user);
    return result.second; // Return true if insertion was successful
}

bool UserDatabase::updateUser(User* user) {
    if (!user) {
        throw invalid_argument("Cannot update null user");
    }
    
    // Find the user by number
    User* existingUser = findUserByNumber(user->getuserNumber());
    if (!existingUser) {
        return false; // User not found
    }
    
    // Remove the existing user (keeping the pointer for later deletion)
    users.erase(existingUser);
    
    // Add the updated user
    users.insert(user);
    
    // Clean up the old user object
    delete existingUser;
    
    return true;
}

/**
 * @brief Remove a user from the database
 * 
 * @param user Pointer to the user to be removed
 * @return true if user was successfully removed
 * @throws UserDatabaseException with appropriate error code if:
 *         - user is null (INVALID_DATA)
 *         - user is the default administrator (PERMISSION_DENIED)
 *         - user is not found in the database (USER_NOT_FOUND)
 */
bool UserDatabase::removeUser(User* user) {
    if (!user) {
        // Use our custom exception with INVALID_DATA error code
        throw UserDatabaseException("Cannot remove null user pointer", 
                               UserDatabaseException::INVALID_DATA);
    }
    
    // Don't allow removing the default administrator (user #10000)
    if (user->getuserNumber() == 10000 && user->getRole() == User::ADMIN) {
        // Use our custom exception with PERMISSION_DENIED error code
        throw UserDatabaseException("Cannot remove the default administrator",
                               UserDatabaseException::PERMISSION_DENIED);
    }
    
    // Find user in set
    auto it = users.find(user);
    if (it == users.end()) {
        // Use our custom exception with USER_NOT_FOUND error code
        throw UserDatabaseException("User not found in database", 
                               UserDatabaseException::USER_NOT_FOUND);
    }
    
    // Remove user from set
    users.erase(it);
    
    // Save changes to file
    saveToFile(filename);
    
    return true;
}

User* UserDatabase::findUserByNumber(u_int32_t userNumber) const {
    for (auto user : users) {
        if (user->getuserNumber() == userNumber) {
            return user;
        }
    }
    return nullptr; // User not found
}

// Implementation of the changeUserPass method (migrated from User::passwd)
bool UserDatabase::changeUserPass(User* activeUser, User* targetUser) {
    if (!activeUser || !targetUser) {
        cerr << "Error: Invalid user pointers." << endl;
        return false;
    }

    // Check if the active user has permission to change the password
    // Only the user itself or an admin can change a password
    if (!(activeUser->getRole() == User::ADMIN
                 || activeUser->getuserNumber() == targetUser->getuserNumber())) {
        cerr << "Error: You don't have permission "
            << "to change this password." << endl;
        return false;
    }

    // First verify current password (unless it's an admin)
    if (!activeUser->getRole() == User::ADMIN 
                 || activeUser->getuserNumber() == targetUser->getuserNumber()) {
        char currentPassword[User::MAX_STR];
        cout << "Enter current password: ";
        cin >> currentPassword;
        
        if (strcmp(currentPassword, targetUser->getPwd().c_str()) != 0) {
            cerr << "Error: Current password is incorrect." << endl;
            return false;
        }
    }
    
    // Request the new password
    char newPassword[User::MAX_STR], confirmPassword[User::MAX_STR];
    
    cout << "Enter new password: ";
    cin >> newPassword;
    
    cout << "Confirm new password: ";
    cin >> confirmPassword;
    
    // Verify passwords match
    if (strcmp(newPassword, confirmPassword) != 0) {
        cerr << "Error: Passwords do not match!" << endl;
        return false;
    }
    
    // Set the new password
    targetUser->setPwd(newPassword);
    
    // cout << "Password changed successfully." << endl;
    return true;
}

std::set<User*> UserDatabase::getAllUsers() const {
    return users;
}