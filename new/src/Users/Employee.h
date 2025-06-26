#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "User.h"

using namespace std;

class Employee : public User {
public:
    Employee(u_int32_t userNumber, 
             const char* nif, 
             const char* pwd, 
             Role role = EMPLOYEE);
    virtual ~Employee() = default;

private:
    // Employee always has role set to EMPLOYEE (1)
    // No need to redefine role variable as it's inherited from User
};

#endif // EMPLOYEE_H