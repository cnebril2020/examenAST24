#include "SystemManager.h"
#include "../Utils/InputUtils.h"
#include "../Users/Admin.h"
#include "../Users/Employee.h"
#include "../Users/UserFactory.h"
#include "../Sensors/TemperatureSensor.h"
#include "../Sensors/Hygrometer.h"
#include "../Sensors/AirQualitySensor.h"
#include "../Sensors/LuxMeterSensor.h"
#include "../Sensors/RGBCamera.h"
#include "../Sensors/ThermalCamera.h"
#include "../Sensors/ContactSensor.h"
#include "../Sensors/SensorFactory.h"
#include "../Databases/Exceptions/UserDatabaseException.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <unistd.h>

using namespace std;

SystemManager::SystemManager(const char* userDbFile, const char* sensorDbFile) 
    : userDB(userDbFile), sensorDB(sensorDbFile), alarmSystem(nullptr), 
      currentUser(nullptr), systemRunning(false) {
}

SystemManager::~SystemManager() {
    delete alarmSystem;
    logout(); // Clean up current user session
}

bool SystemManager::initialize() {
    cout << "=== " << SYSTEM_NAME << " " << VERSION << " ===" << endl;
    cout << "Initializing system components..." << endl;
    
    try {
        // Initialize sensor coordinator with database
        SensorCoordinator::initializeFromDatabase(sensorDB);
        cout << "✓ Sensor coordinator initialized" << endl;
        
        // Create alarm system
        alarmSystem = new AlarmSystem(sensorDB);
        cout << "✓ Security alarm system initialized" << endl;
        
        systemRunning = true;
        cout << "✓ System initialization completed successfully" << endl;
        return true;
        
    } catch (const exception& e) {
        cout << "✗ System initialization failed: " << e.what() << endl;
        return false;
    }
}

void SystemManager::run() {
    displayWelcomeMessage();
    
    while (systemRunning) {
        try {
            if (!currentUser) {
                showGuestMenu();
            } else if (currentUser->getRole() == User::ADMIN) {
                showAdminMenu();
            } else {
                showEmployeeMenu();
            }
        } catch (const exception& e) {
            cout << "System error: " << e.what() << endl;
            cout << "Continuing operation..." << endl;
        }
    }
}

User* SystemManager::authenticate() {
    cout << "\n=== USER AUTHENTICATION ===" << endl;
    
    try {
        u_int32_t userNumber = InputUtils::getNumberInRange(
            "Enter user number (10000-99999): ", 10000, 99999);
        
        string nif = InputUtils::getValidatedString(
            "Enter NIF: ", 
            [](const string& s) { return s.length() <= 8 && !s.empty(); },
            "NIF must be between 1-8 characters");
        
        string password = InputUtils::getValidatedString(
            "Enter password: ",
            [](const string& s) { return !s.empty(); },
            "Password cannot be empty");
        
        currentUser = userDB.login(userNumber, nif.c_str(), password);
        
        bool isAdmin = currentUser->getRole() == User::ADMIN;
        if (currentUser) {
            cout << "✓ Authentication successful" << endl;
            cout << "Welcome, " 
                 << (isAdmin ? "Administrator" : "Employee")
                 << " #" << currentUser->getuserNumber() << endl;
        } else {
            cout << "✗ Authentication failed - Invalid credentials" << endl;
        }
        
    } catch (const exception& e) {
        cout << "Authentication error: " << e.what() << endl;
        currentUser = nullptr;
    }
    
    return currentUser;
}

void SystemManager::logout() {
    if (currentUser) {
        cout << "User #" << currentUser->getuserNumber() << " logged out\n";
        currentUser = nullptr;
    }
}

void SystemManager::showGuestMenu() {
    cout << "\n=== PUBLIC ACCESS ===" << endl;
    cout << "1. 📊 System Status Overview" << endl;
    cout << "2. 👤 User Login" << endl;
    cout << "0. Exit System" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 2);

    switch (choice) {
        case 1:
            displaySystemStatus();
            break;
        case 2:
            authenticate();
            break;
        case 0:
            systemRunning = false;
            cout << "Shutting down system..." << endl;
            break;
    }
}

void SystemManager::showEmployeeMenu() {
    cout << "\n=== EMPLOYEE PANEL ===" << endl;
    cout << "Current user: Employee #" << currentUser->getuserNumber() << endl;
    cout << "1. 📊 Monitoring Dashboard" << endl;
    cout << "2. 🔍 View Sensors (Read-only)" << endl;
    cout << "3. 🚨 Security Status (Read-only)" << endl;
    cout << "4. 🔑 Change My Password" << endl;
    cout << "0. Logout" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 4);
    
    switch (choice) {
        case 1: showMonitoringDashboard(); break;
        case 2: displaySensorList(); break;
        case 3: displaySystemStatus(); break;
        case 4: changeUserPassword(); break;
        case 0: logout(); break;
    }
}

void SystemManager::showAdminMenu() {
    cout << "\n=== ADMINISTRATOR PANEL ===" << endl;
    cout << "Current user: Admin #" << currentUser->getuserNumber() << endl;
    cout << "1. 👥 User Management" << endl;
    cout << "2. 🔧 Sensor Management" << endl;
    cout << "3. 📊 Monitoring Dashboard" << endl;
    cout << "4. 🚨 Security System" << endl;
    cout << "5. ⚙️  System Maintenance" << endl;
    cout << "0. Logout" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 5);
    
    switch (choice) {
        case 1: showUserManagement(); break;
        case 2: showSensorManagement(); break;
        case 3: showMonitoringDashboard(); break;
        case 4: showSecuritySystem(); break;
        case 5: showSystemMaintenance(); break;
        case 0: logout(); break;
    }
}

// === USER MANAGEMENT IMPLEMENTATION ===

void SystemManager::showUserManagement() {
    if (!hasPermission("user_management")) return;
    
    cout << "\n=== USER MANAGEMENT ===" << endl;
    cout << "1. Display all users" << endl;
    cout << "2. Add new user" << endl;
    cout << "3. Update existing user" << endl;
    cout << "4. Remove user" << endl;
    cout << "5. Test user login" << endl;
    cout << "6. Change user password" << endl;
    cout << "7. Clear user database" << endl;
    cout << "8. Test random user access" << endl;
    cout << "9. Test memory allocation exception" << endl;
    cout << "0. Back to main menu" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 9);
    
    switch (choice) {
        case 1: displayUserList(); break;
        case 2: addNewUser(); break;
        case 3: updateExistingUser(); break;
        case 4: removeUser(); break;
        case 5: testUserLogin(); break;
        case 6: changeUserPassword(); break;
        case 7: clearUserDatabase(); break;
        case 8: testRandomUserAccess(); break;
        case 9: testBadAllocException(); break;
        case 0: return;
    }
    
    InputUtils::pauseExecution();
}

void SystemManager::displayUserList() {
    cout << "\n=== USER LIST ===" << endl;
    
    set<User*> users = userDB.getAllUsers();
    if (users.empty()) {
        cout << "No users found in the database." << endl;
        return;
    }
    
    cout << "Total users: " << users.size() << endl;
    cout << "--------------------------------------" << endl;
    
    for (const auto& user : users) {
        displayUserDetails(user);
        cout << "--------------------------------------" << endl;
    }
}

void SystemManager::addNewUser() {
    cout << "\n=== ADD NEW USER ===" << endl;
    
    try {
        u_int32_t userNumber = InputUtils::getNumberInRange(
            "Enter user number (10000-99999): ", 10000, 99999);
        
        string nif = InputUtils::getValidatedString(
            "Enter NIF (up to 8 characters): ",
            [](const string& s) { return s.length() <= 8 && !s.empty(); },
            "NIF must be between 1-8 characters");
        
        string password = InputUtils::getValidatedString(
            "Enter password: ",
            [](const string& s) { return !s.empty(); },
            "Password cannot be empty");
        
        int roleChoice = InputUtils::getNumberInRange(
            "Select role (0=ADMIN, 1=EMPLOYEE): ", 0, 1);
        
        User* newUser = nullptr;
        
        if (roleChoice == 0) {
            newUser = new Admin(userNumber, nif.c_str(), 
                                password.c_str(), User::ADMIN);
        } else {
            newUser = new Employee(userNumber, nif.c_str(), 
                                   password.c_str(), User::EMPLOYEE);
        }
        
        if (userDB.addUser(newUser)) {
            cout << "✓ User added successfully!" << endl;
            displayUserDetails(newUser);
        } else {
            cout << "✗ Failed to add user - User number may already exist\n";
            delete newUser;
        }
        
    } catch (const exception& e) {
        cout << "Error creating user: " << e.what() << endl;
    }
}

void SystemManager::updateExistingUser() {
    cout << "\n=== UPDATE USER ===" << endl;
    
    try {
        u_int32_t userNumber = InputUtils::getNumberInRange(
            "Enter user number to update (10000-99999): ", 10000, 99999);
        
        User* existingUser = userDB.findUserByNumber(userNumber);
        if (!existingUser) {
            cout << "User with number " << userNumber << " not found." << endl;
            return;
        }
        
        cout << "Current user information:" << endl;
        displayUserDetails(existingUser);
        
        string nif = InputUtils::getValidatedString(
            "Enter new NIF (or press Enter to keep current): ",
            [](const string& s) { return s.length() <= 8; },
            "NIF must be 8 characters or less");
        
        string password = InputUtils::getValidatedString(
            "Enter new password (or press Enter to keep current): ",
            [](const string&) { return true; },
            "");
        
        int roleChoice = InputUtils::getNumberInRange(
            "Select new role (0=ADMIN, 1=EMPLOYEE, 2=keep current): ", 0, 2);
        
        // Use existing values if fields were left blank
        const char* newNIF = nif.empty() ? existingUser->getNif() : nif.c_str();
        const char* newPwd = password.empty() ? existingUser->getPwd().c_str() 
                             : password.c_str();
        User::Role newRole = (roleChoice == 2) ? existingUser->getRole() :
                            (roleChoice == 0) ? User::ADMIN : User::EMPLOYEE;
        
        User* updatedUser = nullptr;
        if (newRole == User::ADMIN) {
            updatedUser = new Admin(userNumber, newNIF, newPwd, User::ADMIN);
        } else {
            updatedUser = new Employee(userNumber, newNIF, newPwd, 
                                       User::EMPLOYEE);
        }
        
        if (userDB.updateUser(updatedUser)) {
            cout << "✓ User updated successfully!" << endl;
            displayUserDetails(updatedUser);
        } else {
            cout << "✗ Failed to update user" << endl;
            delete updatedUser;
        }
        
    } catch (const exception& e) {
        cout << "Error updating user: " << e.what() << endl;
    }
}

void SystemManager::removeUser() {
    cout << "\n=== REMOVE USER ===" << endl;
    
    try {
        u_int32_t userNumber = InputUtils::getNumberInRange(
            "Enter user number to remove (10000-99999): ", 10000, 99999);
        
        User* userToRemove = userDB.findUserByNumber(userNumber);
        if (!userToRemove) {
            cout << "User with number " << userNumber << " not found." << endl;
            return;
        }
        
        cout << "User to remove:" << endl;
        displayUserDetails(userToRemove);
        
        string prompt = "Are you sure you want to remove this user?";
        if (InputUtils::getConfirmation(prompt)) {
            if (userDB.removeUser(userToRemove)) {
                cout << "✓ User removed successfully!" << endl;
            } else {
                cout << "✗ Failed to remove user" << endl;
            }
        } else {
            cout << "User removal cancelled." << endl;
        }
        
    } catch (const UserDatabaseException& e) {
        cout << "Database Exception: " << e.what() << endl;
        cout << "Error type: " << e.getErrorString() << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void SystemManager::testUserLogin() {
    cout << "\n=== TEST USER LOGIN ===" << endl;
    
    try {
        u_int32_t userNumber = InputUtils::getNumberInRange(
            "Enter user number: ", 10000, 99999);
        
        string nif = InputUtils::getValidatedString(
            "Enter NIF: ",
            [](const string& s) { return !s.empty(); },
            "NIF cannot be empty");
        
        string password = InputUtils::getValidatedString(
            "Enter password: ",
            [](const string& s) { return !s.empty(); },
            "Password cannot be empty");
        
        User* testUser = userDB.login(userNumber, nif.c_str(), password);
        
        bool isAdmin = testUser->getRole() == User::ADMIN;
        if (testUser) {
            cout << "✓ Login test successful!" << endl;
            cout << "Welcome, " 
                 << (isAdmin ? "Administrator" : "Employee")
                 << " #" << testUser->getuserNumber() << endl;
            displayUserDetails(testUser);
        } else {
            cout << "✗ Login test failed - Invalid credentials" << endl;
        }
        
    } catch (const exception& e) {
        cout << "Login test error: " << e.what() << endl;
    }
}

void SystemManager::changeUserPassword() {
    cout << "\n=== CHANGE USER PASSWORD ===" << endl;
    
    try {
        // For employees, only allow changing their own password
        u_int32_t targetUserNumber;
        if (currentUser->getRole() == User::EMPLOYEE) {
            targetUserNumber = currentUser->getuserNumber();
            cout << "Changing password for current user: " 
                 << targetUserNumber << endl;
        } else {
            targetUserNumber = InputUtils::getNumberInRange(
                "Enter user number whose password to change: ", 10000, 99999);
        }
        
        User* targetUser = userDB.findUserByNumber(targetUserNumber);
        if (!targetUser) {
            cout << "User not found." << endl;
            return;
        }
        
        // Check permissions
        if (currentUser->getRole() != User::ADMIN && 
            currentUser->getuserNumber() != targetUser->getuserNumber()) {
            cout << "Permission denied - ";
            cout << "You can only change your own password." << endl;
            return;
        }
        
        if (userDB.changeUserPass(currentUser, targetUser)) {
            cout << "✓ Password changed successfully." << endl;
        } else {
            cout << "✗ Password change failed." << endl;
        }
        
    } catch (const exception& e) {
        cout << "Password change error: " << e.what() << endl;
    }
}

void SystemManager::clearUserDatabase() {
    if (!hasPermission("database_clear")) return;
    
    cout << "\n=== CLEAR USER DATABASE ===" << endl;
    cout << "WARNING: This will delete all users ";
    cout << "except the default administrator." << endl;
    
    string prompt = "Are you sure you want to clear the user database?";
    if (InputUtils::getConfirmation(prompt)) {
        try {
            if (userDB.clearFile("data/sers.dat")) {
                cout << "✓ User database cleared successfully." << endl;
                cout << "Default administrator preserved." << endl;
            } else {
                cout << "✗ Failed to clear user database." << endl;
            }
        } catch (const exception& e) {
            cout << "Error clearing database: " << e.what() << endl;
        }
    } else {
        cout << "Operation cancelled." << endl;
    }
}

void SystemManager::testRandomUserAccess() {
    cout << "\n=== TEST RANDOM USER ACCESS ===" << endl;
    
    const string filename = "data/sers.dat";
    ifstream file(filename, ios::in | ios::binary);
    
    if (!file.is_open()) {
        cout << "Could not open file " 
             << filename << " for random access test." << endl;
        return;
    }
    
    // Calculate file size and number of records
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    int numRecords = fileSize / sizeof(UserRecord);
    
    cout << "File contains " << numRecords << " user records" << endl;
    
    if (numRecords <= 0) {
        cout << "No records available for random access." << endl;
        file.close();
        return;
    }
    
    // Display first record
    file.seekg(0, ios::beg);
    User* firstUser = UserFactory::readUserFromFile(file);
    
    if (firstUser) {
        cout << "\nFirst record (default admin):" << endl;
        displayUserDetails(firstUser);
        delete firstUser;
    }
    
    // Allow user to access specific records
    if (numRecords > 1) {
        string prompt = "Enter record index to access (0-" 
                         + to_string(numRecords-1) + "): ";
        int recordIndex = InputUtils::getNumberInRange(prompt, 0, numRecords-1);

        file.seekg(recordIndex * sizeof(UserRecord), ios::beg);
        User* user = UserFactory::readUserFromFile(file);
        
        if (user) {
            cout << "\nUser at position " << recordIndex << ":" << endl;
            displayUserDetails(user);
            delete user;
        } else {
            cout << "Failed to read user at position " << recordIndex << endl;
        }
    }
    
    file.close();
}

void SystemManager::testBadAllocException() {
    cout << "\n=== TEST BAD_ALLOC EXCEPTION HANDLING ===" << endl;
    
    const size_t HUGE_SIZE = numeric_limits<size_t>::max() / 10;
    
    try {
        cout << "Attempting to allocate " << HUGE_SIZE << " bytes..." << endl;
        int* hugeArray = new int[HUGE_SIZE];
        
        cout << "Allocation successful (unexpected!)" << endl;
        delete[] hugeArray;
        
    } catch (const bad_alloc& e) {
        cout << "✓ Memory allocation failed as expected!" << endl;
        cout << "Exception details: " << e.what() << endl;
        cout << "This demonstrates proper handling of std::bad_alloc" << endl;
    } catch (const exception& e) {
        cout << "Unexpected exception: " << e.what() << endl;
    }
}

// === SENSOR MANAGEMENT IMPLEMENTATION ===

void SystemManager::showSensorManagement() {
    cout << "\n=== SENSOR MANAGEMENT ===" << endl;
    cout << "1. Display all sensors" << endl;
    cout << "2. Add new sensor" << endl;
    cout << "3. Update existing sensor" << endl;
    cout << "4. Remove sensor" << endl;
    cout << "5. Find sensor by ID" << endl;
    cout << "6. Collect sensor data" << endl;
    cout << "7. Clear sensor database" << endl;
    cout << "8. Test sensor coordination" << endl;
    cout << "0. Back to main menu" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 8);
    
    switch (choice) {
        case 1: displaySensorList(); break;
        case 2: addNewSensor(); break;
        case 3: updateExistingSensor(); break;
        case 4: removeSensor(); break;
        case 5: findSensorById(); break;
        case 6: collectSensorData(); break;
        case 7: clearSensorDatabase(); break;
        case 8: testSensorCoordination(); break;
        case 0: return;
    }
    
    InputUtils::pauseExecution();
}

void SystemManager::displaySensorList() {
    cout << "\n=== SENSOR LIST ===" << endl;
    
    auto sensors = sensorDB.getAllSensors();
    
    if (sensors.empty()) {
        cout << "No sensors in database." << endl;
        return;
    }
    
    cout << "Total sensors: " << sensors.size() << endl;
    cout << "--------------------------------------" << endl;
    
    for (size_t i = 0; i < sensors.size(); ++i) {
        cout << "\n[" << (i + 1) << "/" << sensors.size() << "] ";
        displaySensorDetails(sensors[i]);
        cout << "\n\n--------------------------------------" << endl;
    }
}

void SystemManager::addNewSensor() {
    if (!hasPermission("sensor_management")) return;
    
    cout << "\n=== ADD NEW SENSOR ===" << endl;
    
    try {
        u_int32_t sensorId = InputUtils::getNumberInRange(
            "Enter sensor ID (10000-99999): ", 10000, 99999);
        
        if (sensorDB.findSensorById(sensorId) != nullptr) {
            cout << "Sensor with ID " << sensorId << " already exists!" << endl;
            return;
        }
        
        cout << "\nSelect sensor type:" << endl;
        cout << "0=HYGROMETER, 1=AIR_QUALITY, 2=LUX_METER," << endl;
        cout << "3=TEMPERATURE, 4=CONTACT, 5=THERMAL_CAMERA, 6=RGB_CAMERA\n";
        
        int sensorType = 
                    InputUtils::getNumberInRange("Enter type (0-6): ", 0, 6);
        
        Sensor* newSensor = createSensorByType(sensorType, sensorId);
        
        if (newSensor && sensorDB.addSensor(newSensor)) {
            cout << "✓ Sensor added successfully!" << endl;
            displaySensorDetails(newSensor);
        } else {
            cout << "✗ Failed to add sensor!" << endl;
            delete newSensor;
        }
        
    } catch (const exception& e) {
        cout << "Error adding sensor: " << e.what() << endl;
    }
}

void SystemManager::updateExistingSensor() {
    if (!hasPermission("sensor_management")) return;
    
    cout << "\n=== UPDATE SENSOR ===" << endl;
    cout << "Note: Only sensor ID can be modified. ";
    cout << "Data is updated via collectData()." << endl;
    
    try {
        u_int32_t currentId = InputUtils::getNumberInRange(
            "Enter current sensor ID (10000-99999): ", 10000, 99999);
        
        Sensor* sensor = sensorDB.findSensorById(currentId);
        if (!sensor) {
            cout << "Sensor with ID " << currentId << " not found!" << endl;
            return;
        }
        
        cout << "Current sensor details:" << endl;
        displaySensorDetails(sensor);
        
        if (sensor->isPrimarySensor()) {
            cout << "Cannot modify primary sensor ID! "
                 << "Primary sensors have fixed IDs." << endl;
            return;
        }
        
        u_int32_t newId = InputUtils::getNumberInRange(
            "Enter new sensor ID (10000-99999): ", 10000, 99999);
        
        if (sensorDB.findSensorById(newId) != nullptr) {
            cout << "Error: Sensor with ID " << newId << " already exists!\n";
            return;
        }
        
        if (InputUtils::getConfirmation("Change sensor ID from " 
                                         + to_string(currentId) + 
                                         " to " + to_string(newId) + "?")) {
            sensor->setSensorId(newId);
            
            if (sensorDB.updateSensor(sensor)) {
                cout << "✓ Sensor ID updated successfully!" << endl;
                displaySensorDetails(sensor);
            } else {
                cout << "✗ Failed to update sensor ID!" << endl;
                sensor->setSensorId(currentId); // Restore original ID
            }
        } else {
            cout << "ID change cancelled." << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error updating sensor: " << e.what() << endl;
    }
}

void SystemManager::removeSensor() {
    if (!hasPermission("sensor_management")) return;
    
    cout << "\n=== REMOVE SENSOR ===" << endl;
    
    try {
        u_int32_t sensorId = InputUtils::getNumberInRange(
            "Enter sensor ID to remove (10000-99999): ", 10000, 99999);
        
        Sensor* sensor = sensorDB.findSensorById(sensorId);
        if (!sensor) {
            cout << "Sensor with ID " << sensorId << " not found!" << endl;
            return;
        }
        
        cout << "Sensor to remove:" << endl;
        displaySensorDetails(sensor);
        
        if (sensor->isPrimarySensor()) {
            cout << "Cannot remove primary sensor!" << endl;
            return;
        }
        
        string prompt = "Are you sure you want to remove this sensor?";
        if (InputUtils::getConfirmation(prompt)) {
            if (sensorDB.removeSensor(sensor)) {
                cout << "✓ Sensor removed successfully!" << endl;
            } else {
                cout << "✗ Failed to remove sensor!" << endl;
            }
        } else {
            cout << "Removal cancelled." << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error removing sensor: " << e.what() << endl;
    }
}

void SystemManager::findSensorById() {
    cout << "\n=== FIND SENSOR BY ID ===" << endl;
    
    u_int32_t sensorId = InputUtils::getNumberInRange(
        "Enter sensor ID to find (10000-99999): ", 10000, 99999);
    
    Sensor* sensor = sensorDB.findSensorById(sensorId);
    
    if (sensor) {
        cout << "✓ Sensor found!" << endl;
        displaySensorDetails(sensor);
    } else {
        cout << "✗ Sensor with ID " << sensorId << " not found." << endl;
    }
}

void SystemManager::collectSensorData() {
    cout << "\n=== COLLECT SENSOR DATA ===" << endl;
    
    auto sensors = sensorDB.getAllSensors();
    
    if (sensors.empty()) {
        cout << "No sensors available for data collection." << endl;
        return;
    }
    
    cout << "Starting data collection from " 
         << sensors.size() << " sensor(s)..." << endl;
    cout << "==========================================" << endl;
    
    for (size_t i = 0; i < sensors.size(); ++i) {
        auto sensor = sensors[i];
        
        cout << "\n[" << (i + 1) << "/" << sensors.size() 
             << "] Processing Sensor ID " << sensor->getSensorId();
        
        if (sensor->isPrimarySensor() && 
            (sensor->getType() == Sensor::TEMPERATURE 
             || sensor->getType() == Sensor::CONTACT)) {
            cout << " [MASTER SENSOR]";
        } else if (sensor->isPrimarySensor()) {
            cout << " [PRIMARY SENSOR]";
        }
        cout << endl;
        
        cout << "\n  BEFORE: ";
        displaySensorDetails(sensor);
        
        cout << "  Collecting new data..." << endl;
        sensor->collectData();
        
        cout << "  AFTER:  ";
        displaySensorDetails(sensor);
        
        // Show coordination info for master sensors
        if (sensor->isPrimarySensor()) {
            if (sensor->getType() == Sensor::TEMPERATURE) {
                cout << "  → Global temperature: " 
                     << SensorCoordinator::getGlobalTemperature() << "°C\n";
            }
            if (sensor->getType() == Sensor::CONTACT) {
                cout << "  → Movement status: " 
                     << (SensorCoordinator::isMovementDetected() 
                         ? "DETECTED" : "NONE") << endl;
            }
        }
        
        if (i < sensors.size() - 1) {
            cout << " \n ------------------------------------------" << endl;
            sleep(1); // Brief pause for readability
        }
    }
    
    cout << "\n==========================================" << endl;
    cout << "✓ Data collection completed successfully!" << endl;
    cout << "  Total sensors processed: " << sensors.size() << endl;
}

void SystemManager::clearSensorDatabase() {
    if (!hasPermission("database_clear")) return;
    
    cout << "\n=== CLEAR SENSOR DATABASE ===" << endl;
    cout << "WARNING: This will delete all sensors except primary sensors.\n";
    
    displaySensorList();
    
    string prompt = "Are you sure you want to clear the sensor database?";
    if (InputUtils::getConfirmation(prompt)) {
        try {
            if (sensorDB.clearFile("data/sensors.dat")) {
                cout << "✓ Sensor database cleared successfully!" << endl;
                cout << "Primary sensors preserved." << endl;
                displaySensorList();
            } else {
                cout << "✗ Failed to clear sensor database." << endl;
            }
        } catch (const exception& e) {
            cout << "Error clearing database: " << e.what() << endl;
        }
    } else {
        cout << "Operation cancelled." << endl;
    }
}

void SystemManager::testSensorCoordination() {
    cout << "\n=== SENSOR COORDINATION TEST ===" << endl;
    
    Sensor* masterTemp = sensorDB.findSensorById(
        TemperatureSensor::PRIMARY_TEMP_ID);
    Sensor* masterContact = sensorDB.findSensorById(
        ContactSensor::PRIMARY_CONTACT_ID);
    
    if (!masterTemp || !masterContact) {
        cout << "Error: Missing master sensors for coordination test!" << endl;
        return;
    }
    
    cout << "\nCOORDINATION STATUS:" << endl;
    cout << "==========================================" << endl;
    
    // Temperature coordination
    cout << "TEMPERATURE COORDINATION:" << endl;
    cout << "  Master Sensor #" << TemperatureSensor::PRIMARY_TEMP_ID 
         << " reading: " << masterTemp->getSingleData() << "°C" << endl;
    cout << "  Global Coordinator: " 
         << SensorCoordinator::getGlobalTemperature() << "°C" << endl;
    cout << "  Status: " 
         << (masterTemp->getSingleData() 
             == SensorCoordinator::getGlobalTemperature() ? 
             "SYNCHRONIZED" : "NOT SYNCHRONIZED") << endl;
    
    // Movement coordination
    cout << "\nCONTACT/MOVEMENT COORDINATION:" << endl;
    cout << "  Master Sensor #" << ContactSensor::PRIMARY_CONTACT_ID 
         << " reading: " << masterContact->getSingleData() 
         << " (" << (masterContact->getSingleData() == 1 ? "OPEN" : "CLOSED") 
         << ")" << endl;
    cout << "  Global Coordinator: " 
         << (SensorCoordinator::isMovementDetected() ? "DETECTED" : "NONE");
    bool movementMatch = (masterContact->getSingleData() == 1) 
                          == SensorCoordinator::isMovementDetected();
    cout << "  Status: " << (movementMatch ? "SYNCHRONIZED" :
                             "NOT SYNCHRONIZED") << endl;
    
    cout << "\n==========================================" << endl;
    cout << "Coordination test complete." << endl;
}

// === MONITORING AND SECURITY IMPLEMENTATION ===

void SystemManager::showMonitoringDashboard() {
    cout << "\n=== MONITORING DASHBOARD ===" << endl;
    cout << "1. Display system status" << endl;
    cout << "2. Collect all sensor data" << endl;
    cout << "3. Check security alarm" << endl;
    cout << "4. Test sensor coordination" << endl;
    cout << "0. Back to main menu" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 4);
    
    switch (choice) {
        case 1: displaySystemStatus(); break;
        case 2: collectSensorData(); break;
        case 3: checkSecurityAlarm(); break;
        case 4: testSensorCoordination(); break;
        case 0: return;
    }
    
    InputUtils::pauseExecution();
}

void SystemManager::showSecuritySystem() {
    cout << "\n=== SECURITY SYSTEM ===" << endl;
    cout << "1. Force security check" << endl;
    cout << "2. Display contact sensors" << endl;
    cout << "3. Display camera systems" << endl;
    cout << "0. Back to main menu" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 4);
    
    switch (choice) {
        case 1: checkSecurityAlarm(); break;
        case 2: 
            cout << "\n=== CONTACT SENSORS ===" << endl;
            for (auto sensor : sensorDB.getAllSensors()) {
                if (sensor->getType() == Sensor::CONTACT) {
                    displaySensorDetails(sensor);
                }
            }
            break;
        case 3:
            cout << "\n=== CAMERA SYSTEMS ===" << endl;
            for (auto sensor : sensorDB.getAllSensors()) {
                if (sensor->getType() == Sensor::RGB_CAMERA) {
                    displaySensorDetails(sensor);
                }
            }
            break;
        case 0: return;
    }
    
    InputUtils::pauseExecution();
}

void SystemManager::checkSecurityAlarm() {
    cout << "\n=== SECURITY ALARM CHECK ===" << endl;
    
    if (alarmSystem) {
        bool alarmTriggered = alarmSystem->checkAlarm();
        
        if (alarmTriggered) {
            cout << "🚨 SECURITY ALERT: Movement detected in the system!\n";
            cout << "All RGB cameras have been activated and data captured.\n";
        } else {
            cout << "✅ Security status: All clear - No movement detected\n";
        }
    } else {
        cout << "Error: Alarm system not initialized!" << endl;
    }
}

void SystemManager::displaySystemStatus() {
    cout << "\n=== SYSTEM STATUS OVERVIEW ===" << endl;
    cout << "=========================================" << endl;
    
    auto sensors = sensorDB.getAllSensors();
    auto users = userDB.getAllUsers();
    
    // General statistics
    cout << "📊 SYSTEM STATISTICS:" << endl;
    cout << "  Registered sensors: " << sensors.size() << endl;
    cout << "  Registered users: " << users.size() << endl;
    cout << "  Current user: ";
    if (currentUser) {
        string currentUserNumber = to_string(currentUser->getuserNumber());
        bool isAdmin = currentUser->getRole() == User::ADMIN;
        string currentUserRole = (isAdmin) ? "Administrator" : "Employee";
        cout << "User #" << currentUserNumber << " (" << currentUserRole << ")";
    } else {
        cout << "None (Guest User)";
    }
    cout << endl;
    
    // Environmental status
    cout << "\n🌡️  ENVIRONMENTAL STATUS:" << endl;
    cout << "  Global temperature: " 
         << SensorCoordinator::getGlobalTemperature() << "°C" << endl;
    
    // Security status
    cout << "\n🚨 SECURITY STATUS:" << endl;
    bool movementDetected = SensorCoordinator::isMovementDetected();
    cout << "  Movement detection: " 
         << (movementDetected ? "🔴 DETECTED" : "🟢 NORMAL") << endl;
    
    // Sensor breakdown by type
    cout << "\n📡 SENSOR BREAKDOWN:" << endl;
    int sensorCounts[7] = {0}; // Array for each sensor type
    
    for (auto sensor : sensors) {
        sensorCounts[sensor->getType()]++;
    }
    
    cout << "  Hygrometers: " << sensorCounts[Sensor::HYGROMETER] << endl;
    cout << "  Air Quality: " << sensorCounts[Sensor::AIR_QUALITY] << endl;
    cout << "  Lux Meters: " << sensorCounts[Sensor::LUX_METER] << endl;
    cout << "  Temperature: " << sensorCounts[Sensor::TEMPERATURE] << endl;
    cout << "  Contact: " << sensorCounts[Sensor::CONTACT] << endl;
    cout << "  Thermal Cameras: " << sensorCounts[Sensor::THERMAL_CAMERA];
    cout << "  \nRGB Cameras: " << sensorCounts[Sensor::RGB_CAMERA] << endl;
    
    cout << "\n=========================================" << endl;
}

// === SYSTEM MAINTENANCE IMPLEMENTATION ===

void SystemManager::showSystemMaintenance() {
    if (!hasPermission("system_maintenance")) return;
    
    cout << "\n=== SYSTEM MAINTENANCE ===" << endl;
    cout << "1. Save all databases" << endl;
    cout << "2. Reload databases" << endl;
    cout << "3. Display system statistics" << endl;
    cout << "4. Clear all databases" << endl;
    cout << "5. Initialize default data" << endl;
    cout << "0. Back to main menu" << endl;
    
    int choice = InputUtils::getNumberInRange("Select option: ", 0, 5);
    
    switch (choice) {
        case 1: saveAllDatabases(); break;
        case 2: loadAllDatabases(); break;
        case 3: displaySystemStatistics(); break;
        case 4:
            if (InputUtils::getConfirmation("WARNING: This will \
                                             clear ALL data. Continue?")) {
                clearUserDatabase();
                clearSensorDatabase();
            }
            break;
        case 5:
            if (InputUtils::getConfirmation("Initialize system \
                                             with default data?")) {
                // Re-initialize sensor coordinator
                SensorCoordinator::initializeFromDatabase(sensorDB);
                cout << "✓ Default data initialized" << endl;
            }
            break;
        case 0: return;
    }
    
    InputUtils::pauseExecution();
}

void SystemManager::saveAllDatabases() {
    cout << "\n=== SAVE ALL DATABASES ===" << endl;
    
    try {
        bool userSaved = userDB.saveToFile("data/sers.dat");
        bool sensorSaved = sensorDB.saveToFile("data/sensors.dat");
        
        cout << "User database: " << (userSaved ? "✓ Saved" : "✗ Failed\n");
        cout << "Sensor database: " << (sensorSaved ? "✓ Saved" : "✗ Failed\n");
        
        if (userSaved && sensorSaved) {
            cout << "✓ All databases saved successfully!" << endl;
        } else {
            cout << "✗ Some databases failed to save" << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error saving databases: " << e.what() << endl;
    }
}

void SystemManager::loadAllDatabases() {
    cout << "\n=== RELOAD DATABASES ===" << endl;
    
    try {
        bool userLoaded = userDB.loadFromFile("data/sers.dat");
        bool snsrLoaded = sensorDB.loadFromFile("data/sensors.dat");
        
        cout << "User database: " << (userLoaded ? "✓ Loaded" : "✗ Failed\n");
        cout << "Sensor database: " << (snsrLoaded ? "✓ Loaded" : "✗ Faile\n");
        
        if (userLoaded && snsrLoaded) {
            // Re-initialize sensor coordinator after loading
            SensorCoordinator::initializeFromDatabase(sensorDB);
            cout << "✓ All databases reloaded successfully!" << endl;
        } else {
            cout << "✗ Some databases failed to load" << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error loading databases: " << e.what() << endl;
    }
}

void SystemManager::displaySystemStatistics() {
    cout << "\n=== SYSTEM STATISTICS ===" << endl;
    cout << "=========================================" << endl;
    
    cout << "System Information:" << endl;
    cout << "  Name: " << SYSTEM_NAME << endl;
    cout << "  Version: " << VERSION << endl;
    cout << "  Status: " << (systemRunning ? "Running" : "Stopped") << endl;
    
    cout << "\nMemory Statistics:" << endl;
    cout << "  User count: " << User::getUserCount() << endl;
    cout << "  Sensor count: " << Sensor::getSensorCount() << endl;
    
    cout << "\nDatabase Statistics:" << endl;
    auto users = userDB.getAllUsers();
    auto sensors = sensorDB.getAllSensors();
    cout << "  Users in database: " << users.size() << endl;
    cout << "  Sensors in database: " << sensors.size() << endl;
    
    cout << "\nSensor Coordination:" << endl;
    cout << "  Global temperature: " 
         << SensorCoordinator::getGlobalTemperature() << "°C" << endl;
    cout << "  Movement detected: " 
         << (SensorCoordinator::isMovementDetected() ? "Yes" : "No") << endl;
    
    cout << "\nSession Information:" << endl;
    if (currentUser) {
        cout << "  Current user: #" << currentUser->getuserNumber() << endl;
        cout << "  User role: " 
             << (currentUser->getRole() == User::ADMIN 
                 ? "Administrator" : "Employee");
    } else {
        cout << "  Current user: Guest (not authenticated)" << endl;
    }
    
    cout << "=========================================" << endl;
}

// === UTILITY METHODS IMPLEMENTATION ===

void SystemManager::displayWelcomeMessage() {
    cout << "\n" << string(50, '=') << endl;
    cout << "    " << SYSTEM_NAME << " " << VERSION << endl;
    cout << "    Integrated Monitoring and Security Platform" << endl;
    cout << string(50, '=') << endl;
    cout << "\nFor support, contact: cnebril2020@alumnos.urjc.es" << endl;
    cout << string(50, '=') << endl;
}

void SystemManager::displayUserDetails(const User* user) {
    if (!user) {
        cout << "No user to display." << endl;
        return;
    }
    
    cout << "User #" << user->getuserNumber() 
      << " | NIF: " << user->getNif()
      << " | Role: " << (user->getRole() == User::ADMIN ? "ADMIN" : "EMPLOYEE")
      << " | Password: " << string(user->getPwd().length(), '*') << endl;
}

void SystemManager::displaySensorDetails(const Sensor* sensor) {
    if (!sensor) {
        cout << "Error: NULL sensor pointer" << endl;
        return;
    }
    
    cout << "Sensor ID: " << sensor->getSensorId() << " | ";
    
    // Use operator<< overload for detailed sensor information
    switch (sensor->getType()) {
        case Sensor::HYGROMETER:
            cout << *dynamic_cast<const Hygrometer*>(sensor);
            break;
        case Sensor::AIR_QUALITY:
            cout << *dynamic_cast<const AirQualitySensor*>(sensor);
            break;
        case Sensor::LUX_METER:
            cout << *dynamic_cast<const LuxMeterSensor*>(sensor);
            break;
        case Sensor::TEMPERATURE:
            cout << *dynamic_cast<const TemperatureSensor*>(sensor);
            break;
        case Sensor::CONTACT:
            cout << *dynamic_cast<const ContactSensor*>(sensor);
            break;
        case Sensor::THERMAL_CAMERA:
            cout << *dynamic_cast<const ThermalCamera*>(sensor);
            break;
        case Sensor::RGB_CAMERA:
            cout << *dynamic_cast<const RGBCamera*>(sensor);
            break;
        default:
            cout << "Unknown sensor type | Data: " << sensor->getSingleData();
            break;
    }
    
    if (sensor->isPrimarySensor()) {
        cout << " [PRIMARY]";
    }
}

Sensor* SystemManager::createSensorByType(int sensorType, u_int32_t sensorId) {
    switch (sensorType) {
        case Sensor::HYGROMETER:        return new Hygrometer(sensorId);
        case Sensor::AIR_QUALITY:       return new AirQualitySensor(sensorId);
        case Sensor::LUX_METER:         return new LuxMeterSensor(sensorId);
        case Sensor::TEMPERATURE:       return new TemperatureSensor(sensorId);
        case Sensor::CONTACT:           return new ContactSensor(sensorId);
        case Sensor::THERMAL_CAMERA:    return new ThermalCamera(sensorId);
        case Sensor::RGB_CAMERA:        return new RGBCamera(sensorId);
        default:
            cout << "Error: Unknown Sensor Type" << endl;
            return nullptr;
    }
}

bool SystemManager::hasPermission(const string& operation) const {
    if (!currentUser) {
        cout << "Access denied: Authentication required" << endl;
        return false;
    }
    
    // Admin has access to everything
    if (currentUser->getRole() == User::ADMIN) {
        return true;
    }
    
    // Employee restrictions
    if (operation == "user_management" || 
        operation == "sensor_management" ||
        operation == "database_clear" ||
        operation == "system_maintenance") {
        cout << "Access denied: Administrator privileges required" << endl;
        return false;
    }
    
    return true;
}
