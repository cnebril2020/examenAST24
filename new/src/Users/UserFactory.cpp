#include "UserFactory.h"
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

// Convert User to binary record
UserRecord UserFactory::userToRecord(const User* user) {
    if (!user) {
        throw std::invalid_argument("Cannot convert null user to record");
    }
    UserRecord record;
    record.userNumber = user->getuserNumber();
    // Copy nif with null termination
    strncpy(record.nif, user->getNif(), User::MAX_NIF - 1);
    record.nif[User::MAX_NIF - 1] = '\0';
    // Copy password with null termination
    strncpy(record.pwd, user->getPwd().c_str(), User::MAX_STR - 1);
    record.pwd[User::MAX_STR - 1] = '\0';
    // Set role
    if (user->getRole() == User::ADMIN) {
        record.role = User::ADMIN;
    } else {
        record.role = User::EMPLOYEE;
    }
    return record;
}

// Convert binary record to User
User* UserFactory::recordToUser(const UserRecord& record) {
    try {
        // Convert role int to User object
        if (record.role == User::ADMIN) {
            return new Admin(record.userNumber,
                             record.nif,
                             record.pwd,
                             User::ADMIN);
        } else {
            return new Employee(record.userNumber,
                                record.nif,
                                record.pwd,
                                User::EMPLOYEE);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error creating user from record: " << e.what() << endl;
        return nullptr;
    }
}

// Write User directly to binary file
void UserFactory::writeUserToFile(std::ofstream& file, const User* user) {
    if (!file.is_open()) {
        throw std::runtime_error("File is not open for writing");
    }
    if (!user) {
        throw std::invalid_argument("Cannot write null user to file");
    }
    // Convert user to binary record
    UserRecord record = userToRecord(user);
    // Write record to file
    file.write(reinterpret_cast<char*>(&record), sizeof(UserRecord));
}

/**
 * Reads a user record from a binary file and creates an User object
 * 
 * Uses validation to ensure data integrity
 * 
 * @param file Input file stream to read from (must be open in binary mode)
 * @return Pointer to newly created User object or nullptr if:
 *         - End of file reached
 *         - Invalid/corrupted record detected
 *         - Memory allocation failed
 * @throws std::runtime_error if the file is not open
 * @note Caller is responsible for managing the returned object's memory
 */
User* UserFactory::readUserFromFile(std::ifstream& file) {
    // Verify file is open before attempting to read
    if (!file.is_open()) {
        throw std::runtime_error("File is not open for reading");
    }
    UserRecord record;
    // Read binary record from file
    if (file.read(reinterpret_cast<char*>(&record), sizeof(UserRecord))) {
        // Multiple validations to ensure record integrity
        if (record.userNumber < User::MIN_USER_NUMBER || 
            record.userNumber > User::MAX_USER_NUMBER) {
            return nullptr; // Invalid record: user number out of range
        }
        if (record.role != User::ADMIN && record.role != User::EMPLOYEE) {
            return nullptr; // Invalid record: unrecognized role
        }
        if (record.nif[0] == '\0') {
            return nullptr; // Invalid record: empty nif
        }
        if (record.pwd[0] == '\0') {
            return nullptr; // Invalid record: empty password
        }
        // All validations passed, convert record to User object
        return recordToUser(record);
    }
    
    // End of file or read error occurred
    return nullptr;
}