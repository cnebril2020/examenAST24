#ifndef INPUTUTILS_H
#define INPUTUTILS_H

#include <string>
#include <functional>

// Groups utility functions for input without creating objects
namespace InputUtils {
    // Generic number input with validation
    u_int32_t getNumberInRange(const std::string& prompt, u_int32_t min, u_int32_t max);
    
    // Generic string input with custom validation
    std::string getValidatedString(const std::string& prompt, 
                            std::function<bool(const std::string&)> validator,
                            const std::string& errorMsg);
    
    // Simple yes/no confirmation
    bool getConfirmation(const std::string& prompt);

    void pauseExecution();
}

#endif // INPUTUTILS_H