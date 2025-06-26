#include "User.h"
#include <iostream>

u_int32_t User::userCount = 0;

u_int32_t User::getUserCount() {
    return userCount;
}

// Protected constructor - only derived classes can use it
User::User(u_int32_t userNumber, const char* nif, const char* pwd, Role role) {
    setuserNumber(userNumber); // Use the setter to validate
    setNif(nif);
    setPwd(pwd);
    this->role = role;
    this->userCount++;
}

User::~User() {
    this->userCount--;
}

// Deprecated implementation - redirects to UserDatabase::changeUserPass
bool User::passwd(User &activeuser) {
    (void)activeuser;
    std::cerr << "Warning: User::passwd is deprecated. Use " 
              << "UserDatabase::changeUserPass instead." << std::endl;
    return false; // This method is now deprecated
}

// Implementation of the setuserNumber method
void User::setuserNumber(const u_int32_t& newuserNumber) {
    if (newuserNumber < MIN_USER_NUMBER || newuserNumber > MAX_USER_NUMBER) {
        throw std::invalid_argument("User number must be exactly 5 digits");
    }
    this->userNumber = newuserNumber;
}

// Implementation of the setNif method
void User::setNif(const char* nif) {
    strncpy(this->nif, nif, MAX_NIF - 1);
    this->nif[MAX_NIF - 1] = '\0'; // Ensures null termination
}

// Implementation of setPwd method
void User::setPwd(const char* pwd) {
    strncpy(this->pwd, pwd, MAX_STR - 1);
    this->pwd[MAX_STR - 1] = '\0'; // Ensures null termination
}

// Overloading the << operator for User
std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "User #" << user.getuserNumber() << " (";
    if (user.getRole() == User::ADMIN) {
        os << "Role: ADMIN)" << std::endl;
    } else {
        os << "Role: EMPLOYEE)" << std::endl;
    }
    os << "nif: " << user.getNif() << std::endl;
    os << "Password: " 
        << std::string(strlen(user.getPwd().c_str()), '*') << std::endl;
    
    return os;
}

// Helper function to read user data with error handling
bool readUserData(std::istream &is, u_int32_t &userNumber, char *nif, 
                 char *password, char *roleInt) {
    if (!(is >> userNumber)) {
        std::cerr << "Error: Failed to read user number" << std::endl;
        is.setstate(std::ios::failbit);
        return false;
    }
    
    if (!(is >> nif)) {
        std::cerr << "Error: Failed to read nif" << std::endl;
        is.setstate(std::ios::failbit);
        return false;
    }
    
    if (!(is >> password)) {
        std::cerr << "Error: Failed to read password" << std::endl;
        is.setstate(std::ios::failbit);
        return false;
    }
    
    if (!(is >> roleInt)) {
        std::cerr << "Error: Failed to read role" << std::endl;
        is.setstate(std::ios::failbit);
        return false;
    }
    
    return true;
}

std::istream &operator>>(std::istream &is, User &user) {
    // Check if stream is in a good state before proceeding
    if (!is) return is;

    u_int32_t userNumber;
    char nif[User::MAX_NIF];
    char password[User::MAX_STR];
    char roleInt[User::MAX_STR];
    
    // Read all values with proper error handling
    if (!readUserData(is, userNumber, nif, password, roleInt)) {
        return is; // Error already reported and stream state set
    }
    
    // Validate and set all fields after successful reading
    try {
        user.setuserNumber(userNumber);
        user.setNif(nif);
        user.setPwd(password);
        
        if (strcmp(roleInt, "EMPLOYEE") == 0) user.setRole(User::EMPLOYEE);
        else if (strcmp(roleInt, "ADMIN") == 0) user.setRole(User::ADMIN);
        else {
            std::cerr << "Invalid role value " << roleInt 
                << ". Setting to EMPLOYEE by default.\n";
            user.setRole(User::EMPLOYEE);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error setting user data: " << e.what() << std::endl;
        is.setstate(std::ios::failbit);
    }
    
    return is;
}

bool User::operator<(const User& other) const {
    return this->userNumber < other.userNumber;
}