#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

using namespace std;

class Admin : public User {
public:
    Admin(u_int32_t userNumber, 
          const char* nif, 
          const char* pwd, 
          Role role = ADMIN);
    virtual ~Admin() = default;

private:
    // Admin always has role set to ADMIN (0)
    // No need to redefine role variable as it's inherited from User
};

#endif // ADMIN_H