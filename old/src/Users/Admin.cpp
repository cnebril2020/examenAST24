#include "Admin.h"

Admin::Admin(u_int32_t userNumber, 
             const char* nif, 
             const char* pwd, 
             Role role)
    : User(userNumber, nif, pwd, role) {
    // Constructor delegates to the base class
    // The role parameter has a default value of ADMIN
    // No additional initialization needed as all work is done in the base constructor
}