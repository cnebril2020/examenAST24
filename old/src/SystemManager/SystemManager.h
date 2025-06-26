#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "../Databases/UserDatabase.h"
#include "../Databases/SensorDatabase.h"
#include "../AlarmSystem/AlarmSystem.h"
#include "../Sensors/Coordination/SensorCoordinator.h"
#include "../Users/User.h"
#include "../Sensors/Sensor.h"
#include <string>

/**
 * @brief Unified System Manager for the Julio Veganos e Hijos monitoring system
 * 
 * This class integrates user management, sensor monitoring, and security 
 * systems into a single cohesive interface. It provides role-based access
 * control and comprehensive system administration capabilities.
 * 
 * @author Carlos Nebril
 * @date June 25, 2025
 */
class SystemManager {
public:
    static constexpr const char* SYSTEM_NAME = "Julio Veganos e Hijos System";
    static constexpr const char* VERSION = "v1.0";

    SystemManager(const char* userDbFile = "users.dat", 
                  const char* sensorDbFile = "sensors.dat");
    ~SystemManager();
    
    bool initialize();
    void run();
    User* authenticate();
    void logout();

private:
    // Core system components
    UserDatabase userDB;
    SensorDatabase sensorDB;
    AlarmSystem* alarmSystem;
    User* currentUser;
    bool systemRunning;
    
    // Menu display methods by user role
    void showGuestMenu();
    void showEmployeeMenu();
    void showAdminMenu();

    // === USER MANAGEMENT ===
    void showUserManagement();
    void displayUserList();
    void addNewUser();
    void updateExistingUser();
    void removeUser();
    void testUserLogin();
    void changeUserPassword();
    void clearUserDatabase();
    void testRandomUserAccess();
    void testBadAllocException();
    
    // === SENSOR MANAGEMENT ===
    void showSensorManagement();
    void displaySensorList();
    void addNewSensor();
    void updateExistingSensor();
    void removeSensor();
    void findSensorById();
    void collectSensorData();
    void clearSensorDatabase();
    void testSensorCoordination();
    
    // === MONITORING AND SECURITY ===
    void showMonitoringDashboard();
    void showSecuritySystem();
    void checkSecurityAlarm();
    
    // === SYSTEM MAINTENANCE ===
    void showSystemMaintenance();
    void saveAllDatabases();
    void loadAllDatabases();
    void displaySystemStatistics();
    void displaySystemStatus();

    // Utility and helper methods
    void displayWelcomeMessage();
    void displayUserDetails(const User* user);
    void displaySensorDetails(const Sensor* sensor);
    Sensor* createSensorByType(int sensorType, u_int32_t sensorId);
    bool hasPermission(const std::string& operation) const;
};

#endif // SYSTEM_MANAGER_H