#ifndef USER_H
#define USER_H

#include <string>
#include <cstring>
#include <stdexcept>
#include <ctime>

using namespace std;

// Contains only data from the user. All the operations are contained in 
// 'UserDatabase'.
class User {
public:
    static constexpr size_t MAX_NIF = 9; // 8 characters + '\0'
    static constexpr size_t MAX_STR = 256; // 255 characters + '\0'
    static constexpr size_t MIN_USER_NUMBER = 10000; // Minimum 5-digit number
    static constexpr size_t MAX_USER_NUMBER = 99999; // Maximum 5-digit number

    // Enum for user roles
    enum Role : u_int32_t {
        ADMIN = 0,
        EMPLOYEE = 1
    };
    
    // Abstract Class - Pure Virtual Destructor
    virtual ~User() = 0;    
    
    // Add to User.h - mark passwd as deprecated
    bool passwd(User &activeuser) __attribute__(
        (deprecated("Use UserDatabase::changeUserPass instead")));
        
    // Getters (one-liners kept in header)
    u_int32_t getuserNumber() const { return userNumber; }
    const char* getNif() const { return nif; }
    string getPwd() const { return pwd; }
    Role getRole() const { return role; }
    static u_int32_t getUserCount();
 
    // Setters 
    void setuserNumber(const u_int32_t& newuserNumber);
    void setNif(const char* nif);
    void setPwd(const char* pwd);
    void setRole(Role role) { this->role = role; }

    // Overloaded Operators
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend std::istream& operator>>(std::istream& is, const User& user);
    bool operator<(const User& other) const; // Needed for set<User*>

protected: 
    // Protected constructor - only derived classes can use it
    User(u_int32_t userNumber, const char* nif, const char* pwd, Role role);

    u_int32_t userNumber;
    char nif[MAX_NIF];      // 8 characters + '\0'
    char pwd[MAX_STR];
    Role role;              // Easier to add new type of Users

private:
    // Static variable to count User instances
    static u_int32_t userCount;
};

#endif // USER_H