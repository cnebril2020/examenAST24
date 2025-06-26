#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <limits>
#include "../UserDatabase.h"
#include "../../Users/User.h"
#include "../../Users/Admin.h"
#include "../../Users/Employee.h"
#include "../../Users/UserFactory.h"
#include "../../Utils/InputUtils.h"
#include "../Exceptions/UserDatabaseException.h"

using namespace std;

// Function declarations
void displayMenu();
void displayUserList(const UserDatabase& db);
void testAddUser(UserDatabase& db);
void testUpdateUser(UserDatabase& db);
void testRemoveUser(UserDatabase& db);
void testLogin(UserDatabase& db);
void testPasswordChange(UserDatabase& db);
void testClearFile(UserDatabase& db, const string& filename);
void testRandomAccess(const string& filename);
void testBadAllocException(UserDatabase& db);
void displayUserDetails(const User* user);
void pauseExecution();


/**
 * @file main.cpp
 * @brief User Database Testing Program
 *
 * This program provides a command-line interface to test various features
 * of the UserDatabase class, including CRUD operations, authentication,
 * and exception handling.
 *
 * @author Carlos Nebril
 * @date April 25, 2025
 */

/**
 * @brief Main entry point for the UserDatabase test program
 *
 * Initializes a test database file and provides a menu-driven interface
 * for testing various database operations. The program demonstrates:
 * - User management (add, update, remove)
 * - Authentication (login)
 * - Password management
 * - Exception handling
 * - File operations
 *
 * The database is automatically saved when the program exits.
 *
 * @return int Exit status code (0 for successful execution)
 */
int main() {
    cout << "=== UserDatabase Testing Program ===" << endl;
    
    // Create a test database file
    const char* testFile = "test_users";
    UserDatabase db(testFile);
    
    cout << "UserDatabase initialized with file: " 
         << testFile << ".dat" << endl;
    
    int choice = 0;
    bool exitProgram = false;
    
    // Main program loop
    while (!exitProgram) {
        displayMenu();
        choice = InputUtils::getNumberInRange("Enter your choice (0-9)", 0, 9);

        switch (choice) {
            case 1:
                displayUserList(db);
                break;
                
            case 2:
                testAddUser(db);
                break;
                
            case 3:
                testUpdateUser(db);
                break;
                
            case 4:
                testRemoveUser(db);
                break;
                
            case 5:
                testLogin(db);
                break;
                
            case 6:
                try {
                    testPasswordChange(db);
                }
                catch (const exception& e) {
                    // This will only be reached if an exception escapes testPasswordChange
                    cout << "Unhandled exception in password change operation: " << e.what() << endl;
                }
                break;
                
            case 7:
                testClearFile(db, string(testFile) + ".dat");
                break;
                
            case 8:
                testRandomAccess(string(testFile) + ".dat");
                break;
                
            case 9:
                testBadAllocException(db);  // New test function
                break;

            case 0:
                exitProgram = true;
                cout << "Exiting program. Database will be saved." << endl;
                break;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
        
        if (!exitProgram) {
            pauseExecution();
        }
    }
    
    return 0;
}

/**
 * @brief Displays the main menu options to the user
 *
 * Shows a formatted menu with numbered options for all available
 * operations in the program. Options include user management,
 * authentication testing, and file operations.
 */
void displayMenu() {
    cout << "\n=== Menu Options ===" << endl;
    cout << "1. Display all users" << endl;
    cout << "2. Add a new user" << endl;
    cout << "3. Update an existing user" << endl;
    cout << "4. Remove a user" << endl;
    cout << "5. Test login" << endl;
    cout << "6. Change a user's password" << endl;
    cout << "7. Clear database file" << endl;
    cout << "8. Test random access to user records" << endl;
    cout << "9. Test bad_alloc exception handling" << endl; 
    cout << "0. Exit" << endl;
}

/**
 * @brief Displays a list of all users in the database
 *
 * Retrieves all users from the database and displays their details.
 * If no users are found, displays an appropriate message.
 *
 * @param db Reference to the user database to query
 */
void displayUserList(const UserDatabase& db) {
    cout << "\n=== User List ===" << endl;
    
    set<User*> users = db.getAllUsers();
    if (users.empty()) {
        cout << "No users found in the database." << endl;
        return;
    }
    
    cout << "Number of users: " << users.size() << endl;
    cout << "-----------------------" << endl;
    
    for (const auto& user : users) {
        cout << *user << endl;
        cout << "-----------------------" << endl;
    }
}

/**
 * @brief Test function for adding new users to the database
 *
 * Prompts for user information (number, nif, password, role),
 * creates the appropriate User object, and attempts to add it to
 * the database. Handles potential exceptions during user creation.
 *
 * @param db Reference to the user database
 */
void testAddUser(UserDatabase& db) {
    cout << "\n=== Add New User ===" << endl;
    
    u_int32_t userNumber;
    char nif[User::MAX_NIF];
    char pwd[User::MAX_STR];
    int roleChoice;
    
    // Get user input for new user
    cout << "Enter user number (10000-99999): ";
    cin >> userNumber;
    
    cout << "Enter nif (up to " << User::MAX_NIF - 1 << " characters): ";
    cin >> nif;
    
    cout << "Enter password: ";
    cin >> pwd;
    
    cout << "Select role (0 for ADMIN, 1 for EMPLOYEE): ";
    cin >> roleChoice;
    
    User* newUser = nullptr;
    
    try {
        // Create appropriate user type
        if (roleChoice == 0) {
            newUser = new Admin(userNumber, nif, pwd, User::ADMIN);
        } else {
            newUser = new Employee(userNumber, nif, pwd, User::EMPLOYEE);
        }
        // Try to add user to database
        if (db.addUser(newUser)) {
            cout << "User added successfully!" << endl;
            displayUserDetails(newUser);
        } else {
            cout << "Failed to add user. User with number " 
                 << userNumber << " may already exist." << endl;
            delete newUser; // Clean up if not added to database
        }
    } catch (const exception& e) {
        cout << "Error creating user: " << e.what() << endl;
        if (newUser) delete newUser;
    }
}

/**
 * @brief Test function for updating existing users in the database
 *
 * Prompts for a user number, finds the user, and allows updating
 * nif, password, and/or role. Empty input keeps current values.
 * Creates a new user object with updated information and replaces
 * the existing user in the database.
 *
 * @param db Reference to the user database
 */
void testUpdateUser(UserDatabase& db) {
    cout << "\n=== Update User ===" << endl;
    
    u_int32_t userNumber;
    cout << "Enter user number to update: ";
    cin >> userNumber;
    
    // Try to find user
    User* existingUser = db.findUserByNumber(userNumber);
    if (!existingUser) {
        cout << "User with number " << userNumber << " not found." << endl;
        return;
    }
    
    // Display current user info
    cout << "Current user information:" << endl;
    displayUserDetails(existingUser);
    
    // Get updated info
    char nif[User::MAX_NIF];
    char pwd[User::MAX_STR];
    int roleChoice;
    
    cout << "Enter new nif (or leave blank to keep current): ";
    cin.ignore(); // Clear input buffer
    cin.getline(nif, User::MAX_NIF);
    
    cout << "Enter new password (or leave blank to keep current): ";
    cin.getline(pwd, User::MAX_STR);
    
    cout << "Select new role (0 for ADMIN, 1 for EMPLOYEE, "
         << "or any other number to keep current): ";
    cin >> roleChoice;
    
    // Create updated user object
    User* updatedUser = nullptr;
    try {
        // Use existing values if fields were left blank
        const char* newNIF = (nif[0] == '\0') ? 
             existingUser->getNif() : nif;
        const char* newPwd = (pwd[0] == '\0') ?
             existingUser->getPwd().c_str() : pwd;
        User::Role newRole = (roleChoice == 0) ? User::ADMIN :
                             (roleChoice == 1) ? User::EMPLOYEE :
                             existingUser->getRole();

        // Create appropriate user type
        if (newRole == User::ADMIN) {
            updatedUser = new Admin(userNumber, newNIF, newPwd, User::ADMIN);
        } else {
            updatedUser = new Employee(userNumber, 
                                       newNIF, 
                                       newPwd, 
                                       User::EMPLOYEE);
        }
        
        // Update the user in the database
        if (db.updateUser(updatedUser)) {
            cout << "User updated successfully!" << endl;
            displayUserDetails(updatedUser);
        } else {
            cout << "Failed to update user." << endl;
            delete updatedUser;
        }
    } catch (const exception& e) {
        cout << "Error updating user: " << e.what() << endl;
        if (updatedUser) delete updatedUser;
    }
}

/**
 * @brief Test function for removing users from the database
 * 
 * Demonstrates using and catching custom UserDatabaseException
 * Uses confirmation prompt before removal
 * 
 * @param db Reference to the user database
 */
void testRemoveUser(UserDatabase& db) {
    cout << "\n=== Remove User ===" << endl;

    u_int32_t userNumber = InputUtils::getNumberInRange(
        "Enter a user number (10000-99999): ", 10000, 99999);    
    User* userToRemove = db.findUserByNumber(userNumber);
    if (!userToRemove) {
        cout << "User with number " << userNumber << " not found." << endl;
        return;
    }
    
    cout << "User to remove:" << endl;
    displayUserDetails(userToRemove);
    
    string prompt = "Are you sure you want to remove this user?";
    if (InputUtils::getConfirmation(prompt)) {
        try {
            if (db.removeUser(userToRemove)) {
                cout << "User removed successfully!" << endl;
            } else {
                cout << "Failed to remove user." << endl;
            }
        } catch (const UserDatabaseException& e) {
            cout << "Database Exception: " << e.what() << endl;
            cout << "Error type: " << e.getErrorString() << endl;
            cout << "Error code: " << e.getErrorCode() << endl;
        } catch (const std::exception& e) {
            // Fallback for other types of exceptions
            cout << "Error: " << e.what() << endl;
        }
    } else {
        cout << "User removal cancelled." << endl;
    }
}

/**
 * @brief Test function for user login verification
 *
 * Prompts for login credentials (user number, nif, password)
 * and attempts to authenticate the user. Displays success or failure message
 * and shows user details if login is successful.
 *
 * @param db Reference to the user database
 */
void testLogin(UserDatabase& db) {
    cout << "\n=== User Login ===" << endl;
    
    u_int32_t userNumber;
    char nif[User::MAX_NIF];
    char pwd[User::MAX_STR];
    
    cout << "Enter user number: ";
    cin >> userNumber;
    
    cout << "Enter nif: ";
    cin >> nif;
    
    cout << "Enter password: ";
    cin >> pwd;
    
    // Attempt login
    string passwordStr(pwd);
    User* loggedInUser = db.login(userNumber, nif, passwordStr);
    
    if (loggedInUser) {
        cout << "Login successful!" << endl;
        cout << "Welcome, " 
             << (loggedInUser->getRole() == User::ADMIN
                 ? "Administrator" : "Employee") 
             << " #" << loggedInUser->getuserNumber() << endl;
        displayUserDetails(loggedInUser);
    } else {
        cout << "Login failed. Invalid credentials." << endl;
    }
}

/**
 * @brief Tests the password change functionality with exception re-throwing
 * 
 * This function demonstrates exception re-throwing by:
 * 1. Catching authentication exceptions
 * 2. Logging information about the error
 * 3. Re-throwing the exception to be handled at a higher level
 * 
 * @param db Reference to the user database
 */
void testPasswordChange(UserDatabase& db) {
    cout << "\n=== Change User Password ===" << endl;
    
    // First, login to get an active user
    u_int32_t activeuserNumber;
    char activeNIF[User::MAX_NIF];
    char activePwd[User::MAX_STR];
    
    try {
        cout << "First, please login:" << endl;
        cout << "Enter your user number: ";
        cin >> activeuserNumber;
        
        cout << "Enter your nif: ";
        cin >> activeNIF;
        
        cout << "Enter your password: ";
        cin >> activePwd;
        
        // Attempt login
        string passwordStr(activePwd);
        User* activeUser = nullptr;
        
        try {
            activeUser = db.login(activeuserNumber, activeNIF, passwordStr);
            
            if (!activeUser) {
                throw runtime_error("Login failed: Invalid credentials");
            }
            
            // Get the user whose password will be changed
            u_int32_t targetuserNumber;
            cout << "Enter the user number whose password you want to change: ";
            cin >> targetuserNumber;
            
            User* targetUser = db.findUserByNumber(targetuserNumber);
            if (!targetUser) {
                throw runtime_error("User not found");
            }
            
            // Check if active user has permission
            if (activeUser->getRole() != User::ADMIN && 
                activeUser->getuserNumber() != targetUser->getuserNumber()) {
                throw runtime_error("Permission denied");
            }
            
            // Attempt password change
            if (db.changeUserPass(activeUser, targetUser)) {
                cout << "Password changed successfully." << endl;
            } else {
                throw runtime_error("Password change operation failed");
            }
        }
        catch (const runtime_error& e) {
            // Log the error details
            cout << "Authentication error: " << e.what() << endl;
            cout << "User attempted: " << activeuserNumber << endl;
            
            // Example of re-throwing the same exception
            // This will be caught by the outer try-catch block
            cout << "Re-throwing exception..." << endl;
            throw;  // Re-throw the current exception
        }
    }
    catch (const exception& e) {
        // Handle the re-thrown exception
        cout << "Operation aborted: " << e.what() << endl;
        cout << "Please try again with valid credentials." << endl;
    }
}

/**
 * @brief Test function for clearing the database file
 *
 * Prompts for confirmation and then removes all users from the database file,
 * except for the default administrator. Handles potential exceptions during
 * the clear operation.
 *
 * @param db Reference to the user database
 * @param filename The name of the database file to clear
 */
void testClearFile(UserDatabase& db, const string& filename) {
    cout << "\n=== Clear Database File ===" << endl;
    
    char confirm;
    cout << "WARNING: This will delete all users from the file." << endl;
    cout << "Are you sure? (y/n): ";
    cin >> confirm;
    
    if (tolower(confirm) == 'y') {
        try {
            if (db.clearFile(filename.c_str())) {
                cout << "Database file cleared successfully." << endl;
            } else {
                cout << "Failed to clear database file." << endl;
            }
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }
    } else {
        cout << "Operation cancelled." << endl;
    }
}

/**
 * @brief Test function for random access to user records in binary file
 *
 * Opens the database file in binary mode and allows direct access
 * to user records by index. Shows file statistics including the number
 * of records stored. Demonstrates low-level binary file operations.
 *
 * @param filename The name of the database file to access
 */
void testRandomAccess(const string& filename) {
    cout << "\n=== Testing Random Binary Access ===" << endl;
    
    // Open the binary file for reading
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cout << "Could not open file " << filename 
             << " for random access test." << endl;
        return;
    }
    
    // Calculate file size
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    int numRecords = fileSize / sizeof(UserRecord);
    
    cout << "File contains " << numRecords << " user records" << endl;
    
    // Check if there are any records
    if (numRecords <= 0) {
        cout << "No records available for random access." << endl;
        file.close();
        return;
    }
    
    // Display the first record (Admin user)
    file.seekg(0, ios::beg);
    User* firstUser = UserFactory::readUserFromFile(file);
    
    if (firstUser) {
        cout << "First record (default admin):" << endl;
        cout << *firstUser << endl;
        delete firstUser;
    } else {
        cout << "Failed to read first record." << endl;
    }
    
    // If more records exist, access random records
    if (numRecords > 1) {
        // Let the user choose a record to access
        int recordIndex;
        while (true) {
            cout << "Enter a record index to access (0-" << numRecords - 1 
                 << ", or -1 to exit): ";
            cin >> recordIndex;
            
            if (recordIndex == -1) {
                break;
            }
            
            if (recordIndex < 0 || recordIndex >= numRecords) {
                cout << "Invalid record index." << endl;
                continue;
            }
            
            // Seek to the selected record
            file.seekg(recordIndex * sizeof(UserRecord), ios::beg);
            
            // Read the user from this position
            User* user = UserFactory::readUserFromFile(file);
            
            if (user) {
                cout << "User at position " << recordIndex << ":" << endl;
                cout << *user << endl;
                delete user;
            } else {
                cout << "Failed to read user at position " 
                     << recordIndex << endl;
            }
        }
    }
    
    file.close();
}


/**
 * @brief Test function to demonstrate handling of std::bad_alloc exceptions
 * 
 * This function deliberately attempts to allocate an excessive amount of memory
 * to trigger a std::bad_alloc exception, then shows proper exception handling.
 * 
 * @param db Reference to the user database (not used but kept for consistency)
 */
void testBadAllocException(UserDatabase& db) {
    cout << "\n=== Testing std::bad_alloc Exception Handling ===" << endl;
    
    // Size that will likely trigger a bad_alloc exception on most systems
    // Using size_t max value effectively requests all available memory
    const size_t HUGE_SIZE = std::numeric_limits<size_t>::max() / 10;
    
    try {
        cout << "Attempting to allocate " << HUGE_SIZE 
             << " bytes of memory..." << endl;
        
        // This will almost certainly fail and throw std::bad_alloc
        int* hugeArray = new int[HUGE_SIZE];
        
        // This code should never execute if the allocation fails as expected
        cout << "Allocation successful (unexpected!)" << endl;
        delete[] hugeArray;  // Clean up if allocation somehow succeeds
    }
    catch (const std::bad_alloc& e) {
        // Specific handler for memory allocation failure
        cout << "Memory allocation failed as expected!" << endl;
        cout << "Exception details: " << e.what() << endl;
        cout << "This demonstrates proper handling of std::bad_alloc" << endl;
    }
    catch (const std::exception& e) {
        // Generic handler for other exceptions
        cout << "Unexpected exception: " << e.what() << endl;
    }
    
    // Parameter is not used but kept for consistency with other test functions
    (void)db;
}

/**
 * @brief Displays detailed information about a user
 *
 * Shows user number, nif, masked password, and role.
 * Handles null user pointer gracefully by showing an error message.
 *
 * @param user Pointer to the User object to display
 */
void displayUserDetails(const User* user) {
    if (!user) {
        cout << "No user to display." << endl;
        return;
    }
    
    cout << "---------------------" << endl;
    cout << "User Number: " << user->getuserNumber() << endl;
    cout << "nif: " << user->getNif() << endl;
    cout << "Password: " << string(strlen(user->getPwd().c_str()), '*') << endl;
    cout << "Role: " << (user->getRole() == User::ADMIN ? "ADMIN" : "EMPLOYEE");
    cout << "\n---------------------" << endl;
}

/**
 * @brief Pauses program execution until user presses Enter
 *
 * Provides a clean way to pause the program between operations,
 * allowing the user to view results before continuing. Handles
 * input buffer clearing to ensure consistent behavior regardless
 * of previous input operations.
 */
void pauseExecution() {
    cout << "\nPress Enter to continue...";
    
    // Clean input buffer and wait for Enter
    if (cin.peek() == '\n') {
        // If there's a pending newline character, consume it
        cin.get();
    } else {
        // If there's no pending newline or other characters in buffer,
        // clear the entire buffer up to the next newline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}