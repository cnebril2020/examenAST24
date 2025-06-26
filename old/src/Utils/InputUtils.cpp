#include "InputUtils.h"
#include <iostream>
#include <limits>

namespace InputUtils {
    
    u_int32_t getNumberInRange(const std::string& prompt, 
                               u_int32_t min, 
                               u_int32_t max) {
        u_int32_t value;
        std::string input;
        
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            
            try {
                unsigned long temp = std::stoul(input);
                if (temp > std::numeric_limits<u_int32_t>::max()) {
                    throw std::out_of_range("Value too large");
                }
                value = static_cast<u_int32_t>(temp);
                
                if (value >= min && value <= max) {
                    return value;
                } else {
                    std::cout << "Value must be between " << min 
                              << " and " << max << std::endl;
                }
            } catch (const std::exception&) {
                std::cout << "Invalid input. Please enter a valid number." 
                          << std::endl;
            }
        }
    }
    
    std::string getValidatedString(const std::string& prompt, 
                                   std::function<bool(const std::string&)> validator,
                                   const std::string& errorMsg) {
        std::string input;
        
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            
            if (validator(input)) {
                return input;
            } else {
                std::cout << errorMsg << std::endl;
            }
        }
    }
    
    bool getConfirmation(const std::string& prompt) {
        std::string input;
        
        while (true) {
            std::cout << prompt << " (y/n): ";
            std::getline(std::cin, input);
            
            if (input.length() == 1) {
                char c = std::tolower(input[0]);
                if (c == 'y') return true;
                if (c == 'n') return false;
            }
            std::cout << "Please enter 'y' for yes or 'n' for no." 
                      << std::endl;
        }
    }

    void pauseExecution() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}