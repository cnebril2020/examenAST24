#include "Employee.h"

Employee::Employee(u_int32_t userNumber, 
                   const char* NIF, 
                   const char* pwd, 
                   Role role)
    : User(userNumber, NIF, pwd, role) {
    // Constructor delegates to the base class
    // The role parameter has a default value of EMPLOYEE
    // No additional initialization needed as all work is done in the base constructor
}