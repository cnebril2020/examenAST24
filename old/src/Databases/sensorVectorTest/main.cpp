#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <unistd.h>
#include "../SensorDatabase.h"
#include "../../Sensors/Sensor.h"
#include "../../Sensors/TemperatureSensor.h"
#include "../../Sensors/Hygrometer.h"
#include "../../Sensors/AirQualitySensor.h"
#include "../../Sensors/LuxMeterSensor.h"
#include "../../Sensors/RGBCamera.h"
#include "../../Sensors/ThermalCamera.h"
#include "../../Sensors/ContactSensor.h"
#include "../../Sensors/SensorFactory.h"
#include "../../Sensors/Coordination/SensorCoordinator.h"
#include "../../AlarmSystem/AlarmSystem.h"
#include "../../Utils/InputUtils.h"

using namespace std;

// Function declarations
void displayMenu();
void displaySensorList(const SensorDatabase& db);
Sensor* createSensorByType(int sensorType, u_int32_t sensorId);
void testAddSensor(SensorDatabase& db);
void testUpdateSensor(SensorDatabase& db);
void testRemoveSensor(SensorDatabase& db);
void testFindSensor(SensorDatabase& db);
void testCollectData(SensorDatabase& db);
void testClearFile(SensorDatabase& db, const string& filename);
void displaySensorDetails(const Sensor* sensor);
void testSensorCoordination(SensorDatabase& db);
void pauseExecution();

/**
 * @file main.cpp
 * @brief Sensor Database Testing Program
 *
 * This program provides a command-line interface to test various features
 * of the SensorDatabase class, including CRUD operations, data collection,
 * and exception handling.
 *
 * @author Carlos Nebril
 * @date June 16, 2025
 */

/**
 * @brief Main entry point for the SensorDatabase test program
 *
 * Initializes a test database file and provides a menu-driven interface
 * for testing various database operations. The program demonstrates:
 * - Sensor management (add, update, remove)
 * - Data collection from sensors
 * - Primary sensor protection
 * - Exception handling
 * - File operations
 *
 * The database is automatically saved when the program exits.
 *
 * @return int Exit status code (0 for successful execution)
 */
int main() {
    cout << "=== SensorDatabase Testing Program ===" << endl;
    
    // Create a test database file
    const char* testFile = "test_sensors";
    SensorDatabase db(testFile);
    
    cout << "SensorDatabase initialized with file: " 
         << testFile << ".dat" << endl;
    

    SensorCoordinator::initializeFromDatabase(db);

    int choice = 0;
    bool exitProgram = false;
    
    // Main program loop
    while (!exitProgram) {
        displayMenu();
        choice = InputUtils::getNumberInRange("Enter your choice: ", 0, 9);
        switch (choice) {
            case 1:
                displaySensorList(db);
                break;
                
            case 2:
                testAddSensor(db);
                break;
                
            case 3:
                testUpdateSensor(db);
                break;
                
            case 4:
                testRemoveSensor(db);
                break;
                
            case 5:
                testFindSensor(db);
                break;
                
            case 6:
                testCollectData(db);
                break;
                
            case 7:
                testClearFile(db, string(testFile) + ".dat");
                break;
            
            case 8:
                testSensorCoordination(db);
                break;
                
            case 9: {
                AlarmSystem alarm(db);
                alarm.checkAlarm();
                break;
            }

            case 0:
                exitProgram = true;
                cout << "Exiting program. Database will be saved." << endl;
                break;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
        
        if (!exitProgram) {
            pauseExecution();
        }
    }
    
    return 0;
}

/**
 * @brief Displays the main menu options to the user
 *
 * Shows a formatted menu with numbered options for all available
 * operations in the program. Options include sensor management,
 * data collection testing, and file operations.
 */
void displayMenu() {
    cout << "\n=================== MENU ===================" << endl;
    cout << "1. Display all sensors" << endl;
    cout << "2. Add new sensor" << endl;
    cout << "3. Update existing sensor" << endl;
    cout << "4. Remove sensor" << endl;
    cout << "5. Find sensor by ID" << endl;
    cout << "6. Test data collection" << endl;
    cout << "7. Clear database file" << endl;
    cout << "8. Test SensorCoordinator integration" << endl;
    cout << "9. Check Security Alarm" << endl;
    cout << "0. Exit" << endl;
    cout << "=============================================" << endl;
}

/**
 * @brief Displays all sensors currently in the database
 *
 * Shows a formatted list of all sensors with their details.
 * If the database is empty, displays an appropriate message.
 *
 * @param db Reference to the SensorDatabase to display
 */
void displaySensorList(const SensorDatabase& db) {
    cout << "\n=== SENSOR LIST ===" << endl;
    
    auto sensors = db.getAllSensors();
    
    if (sensors.empty()) {
        cout << "No sensors in database." << endl;
        return;
    }
    
    cout << "Total sensors: " << sensors.size() << endl;
    cout << "-----------------------" << endl;
    
    int i = 0;
    for (const auto& sensor : sensors) {
        cout << "\n[" << (i + 1) << "/" << sensors.size() << "] ";
        displaySensorDetails(sensor);
        cout << "\n-----------------------" << endl;
        i++;
    }
}

/**
 * @brief Creates a sensor object based on the specified type
 *
 * Factory function that creates and returns a sensor instance of the
 * appropriate type. Handles all supported sensor types.
 *
 * @param sensorType Integer representing the sensor type (0-6)
 * @param sensorId Unique identifier for the sensor
 * @return Sensor* Pointer to the created sensor object, or nullptr if type is invalid
 */
Sensor* createSensorByType(int sensorType, u_int32_t sensorId) {
    switch (sensorType) {
        case Sensor::HYGROMETER:        // 0
            return new Hygrometer(sensorId);
        case Sensor::AIR_QUALITY:       // 1
            return new AirQualitySensor(sensorId);
        case Sensor::LUX_METER:         // 2
            return new LuxMeterSensor(sensorId);
        case Sensor::TEMPERATURE:       // 3
            return new TemperatureSensor(sensorId);
        case Sensor::CONTACT:           // 4
            return new ContactSensor(sensorId);
        case Sensor::THERMAL_CAMERA:    // 5
            return new ThermalCamera(sensorId);
        case Sensor::RGB_CAMERA:        // 6
            return new RGBCamera(sensorId);
        default:
            cout << "Error: Unkown Sensor Type" << endl;
            return nullptr;
    }
}

/**
 * @brief Test function for adding new sensors to the database
 *
 * Uses InputUtils for validated input of sensor ID and type.
 * Demonstrates input validation and error handling for duplicate sensor IDs.
 *
 * @param db Reference to the SensorDatabase for testing
 */
void testAddSensor(SensorDatabase& db) {
    cout << "\n=== ADD SENSOR TEST ===" << endl;
    
    try {
        // Use InputUtils for validated sensor ID input
        u_int32_t sensorId = InputUtils::getNumberInRange(
            "Enter sensor ID (10000-99999): ", 10000, 99999);
        
        // Check if sensor already exists before asking for type
        if (db.findSensorById(sensorId) != nullptr) {
            cout << "Sensor with ID " << sensorId << " already exists!\n";
            return;
        }
        
        // Display sensor type menu and get validated input
        cout << "\nSelect sensor type:" << endl;
        cout << "0=HYGROMETER, 1=AIR_QUALITY, 2=LUX_METER," << endl;
        cout << "3=TEMPERATURE, 4=CONTACT, 5=THERMAL_CAMERA, 6=RGB_CAMERA\n";
        
        int sensorType = static_cast<int>(InputUtils::getNumberInRange(
            "Enter type (0-6): ", 0, 6));
        
        Sensor* newSensor = createSensorByType(sensorType, sensorId);
        
        if (db.addSensor(newSensor)) {
            cout << "Sensor added successfully!" << endl;
            displaySensorDetails(newSensor);
        } else {
            cout << "Failed to add sensor!" << endl;
            delete newSensor;
        }
        
    } catch (const exception& e) {
        cout << "Exception caught: " << e.what() << endl;
    }
}

/**
 * @brief Test function for updating existing sensors
 *
 * Allows updating only the sensor ID (hardware reconfiguration).
 * Sensor data should only be modified through collectData(), not manually.
 *
 * @param db Reference to the SensorDatabase for testing
 */
void testUpdateSensor(SensorDatabase& db) {
    cout << "\n=== UPDATE SENSOR TEST ===" << endl;
    cout << "Note: Only sensor ID can be modified. Sensor data is updated automatically via collectData()." << endl;
    
    try {
        // Get current sensor
        u_int32_t currentId = InputUtils::getNumberInRange(
            "Enter current sensor ID to update (10000-99999): ", 10000, 99999);
        
        Sensor* sensor = db.findSensorById(currentId);
        if (!sensor) {
            cout << "Sensor with ID " << currentId << " not found!" << endl;
            return;
        }
        
        cout << "Current sensor details:" << endl;
        displaySensorDetails(sensor);
        
        // Check primary sensor restriction
        if (sensor->isPrimarySensor()) {
            cout << "Cannot modify primary sensor ID! Primary sensors have fixed IDs for system coordination." << endl;
            return;
        }
        
        // Get and validate new ID
        u_int32_t newId = InputUtils::getNumberInRange(
            "Enter new sensor ID (10000-99999): ", 10000, 99999);
        
        if (db.findSensorById(newId) != nullptr) {
            cout << "Error: Sensor with ID " << newId << " already exists!" << endl;
            return;
        }
        
        // Confirm and update
        cout << "\nChanging sensor ID from " << currentId << " to " << newId << endl;
        
        if (InputUtils::getConfirmation("Are you sure you want to change the sensor ID?")) {
            // Simply update the sensor ID directly - no need to remove/re-add
            sensor->setSensorId(newId);
            
            // Use updateSensor to refresh the database state
            if (db.updateSensor(sensor)) {
                cout << "Sensor ID updated successfully!" << endl;
                cout << "\nUpdated sensor details:" << endl;
                displaySensorDetails(sensor);
                cout << "\nNote: Use 'Test data collection' to get new measurements." << endl;
            } else {
                cout << "Failed to update sensor ID!" << endl;
                // Restore original ID on failure
                sensor->setSensorId(currentId);
            }
        } else {
            cout << "ID change cancelled." << endl;
        }
        
    } catch (const exception& e) {
        cout << "Exception caught: " << e.what() << endl;
    }
}

/**
 * @brief Test function for removing sensors from the database
 *
 * Uses InputUtils for validated input and confirmation.
 * Demonstrates sensor removal with protection for primary sensors.
 *
 * @param db Reference to the SensorDatabase for testing
 */
void testRemoveSensor(SensorDatabase& db) {
    cout << "\n=== REMOVE SENSOR TEST ===" << endl;
    
    try {
        // Use InputUtils for validated sensor ID input
        u_int32_t sensorId = InputUtils::getNumberInRange(
            "Enter sensor ID to remove (10000-99999): ", 10000, 99999);
        
        Sensor* sensor = db.findSensorById(sensorId);
        if (!sensor) {
            cout << "Sensor with ID " << sensorId << " not found!" << endl;
            return;
        }
        
        cout << "Sensor to remove:" << endl;
        displaySensorDetails(sensor);
        
        // Check if it's a primary sensor
        if (sensor->isPrimarySensor()) {
            cout << "Cannot remove primary sensor!" << endl;
            return;
        }
        
        // Use InputUtils for confirmation
        if (InputUtils::getConfirmation("Are you sure you want to remove this sensor?")) {
            if (db.removeSensor(sensor)) {
                cout << "Sensor removed successfully!" << endl;
            } else {
                cout << "Failed to remove sensor!" << endl;
            }
        } else {
            cout << "Removal cancelled." << endl;
        }
        
    } catch (const exception& e) {
        cout << "Exception caught: " << e.what() << endl;
    }
}

/**
 * @brief Test function for finding sensors by ID
 *
 * Uses InputUtils for validated sensor ID input.
 * Tests the sensor search functionality and displays results.
 *
 * @param db Reference to the SensorDatabase for testing
 */
void testFindSensor(SensorDatabase& db) {
    cout << "\n=== FIND SENSOR TEST ===" << endl;
    
    // Use InputUtils for validated sensor ID input
    u_int32_t sensorId = InputUtils::getNumberInRange(
        "Enter sensor ID to find (10000-99999): ", 10000, 99999);
    
    Sensor* sensor = db.findSensorById(sensorId);
    
    if (sensor) {
        cout << "Sensor found!" << endl;
        displaySensorDetails(sensor);
    } else {
        cout << "Sensor with ID " << sensorId << " not found." << endl;
    }
}

/**
 * @brief Test function for sensor data collection
 *
 * Tests the collectData() method for all sensors in the database
 * and displays the results with detailed information.
 *
 * @param db Reference to the SensorDatabase for testing
 */
void testCollectData(SensorDatabase& db) {
    cout << "\n=== DATA COLLECTION TEST ===" << endl;
    
    auto sensors = db.getAllSensors();
    
    if (sensors.empty()) {
        cout << "No sensors available for data collection." << endl;
        return;
    }
    
    cout << "Starting data collection from " << sensors.size() << " sensor(s)..." << endl;
    cout << "This will simulate taking new measurements from each sensor." << endl;
    cout << "==========================================" << endl;
    
    for (size_t i = 0; i < sensors.size(); ++i) {
        auto sensor = sensors[i];
        
        cout << "\n[" << (i + 1) << "/" << sensors.size() << "] Processing Sensor ID " 
             << sensor->getSensorId();
        
        if (sensor->isPrimarySensor() && 
            (sensor->getType() == Sensor::TEMPERATURE || sensor->getType() == Sensor::CONTACT)) {
            cout << " [MASTER SENSOR]";
        } else if (sensor->isPrimarySensor()) {
            cout << " [PRIMARY SENSOR]";
        }
        cout << endl;
        
        cout << "  BEFORE collection: ";
        displaySensorDetails(sensor);
        
        cout << "  Collecting new data..." << endl;
        sensor->collectData();
        
        cout << "  AFTER collection:  ";
        displaySensorDetails(sensor);
        
        // Show coordination info for master sensors
        if (sensor->isPrimarySensor()) {
            if (sensor->getType() == Sensor::TEMPERATURE) {
                cout << "  → Global temperature updated to: " 
                     << SensorCoordinator::getGlobalTemperature() << "°C" << endl;
            }
            if (sensor->getType() == Sensor::CONTACT) {
                cout << "  → Movement status updated to: " 
                     << (SensorCoordinator::isMovementDetected() ? "DETECTED" : "NONE") << endl;
            }
        }
        
        if (i < sensors.size() - 1) {
            cout << "  ------------------------------------------" << endl;
            sleep(1);  // Brief pause between sensors for readability
        }
    }
    
    cout << "\n==========================================" << endl;
    cout << "✓ Data collection completed successfully!" << endl;
    cout << "  Total sensors processed: " << sensors.size() << endl;
    cout << "  All measurements have been updated." << endl;
}

/**
 * @brief Test function for clearing the database file
 *
 * Uses InputUtils for confirmation input.
 * Demonstrates file clearing functionality while preserving primary sensors.
 *
 * @param db Reference to the SensorDatabase for testing
 * @param filename Name of the database file to clear
 */
void testClearFile(SensorDatabase& db, const string& filename) {
    cout << "\n=== CLEAR FILE TEST ===" << endl;
    
    cout << "Sensors before clearing:" << endl;
    displaySensorList(db);
    
    // Use InputUtils for confirmation
    if (InputUtils::getConfirmation("Are you sure you want to clear the database file?\n(Primary sensors will be preserved)")) {
        try {
            if (db.clearFile(filename.c_str())) {
                cout << "Database file cleared successfully!" << endl;
                cout << "Primary sensors preserved." << endl;
                
                cout << "\nSensors after clearing:" << endl;
                displaySensorList(db);
            } else {
                cout << "Failed to clear database file." << endl;
            }
        } catch (const exception& e) {
            cout << "Exception during file clearing: " << e.what() << endl;
        }
    } else {
        cout << "File clearing cancelled." << endl;
    }
}

/**
 * @brief Displays detailed information about a sensor
 *
 * Shows sensor ID, type, data value, and primary status.
 * Handles null sensor pointer gracefully by showing an error message.
 *
 * @param sensor Pointer to the Sensor object to display
 */
void displaySensorDetails(const Sensor* sensor) {
    if (!sensor) {
        cout << "Error: NULL sensor pointer" << endl;
        return;
    }
    
    cout << "Sensor ID: " << sensor->getSensorId() << " | ";
    
    // Use operator<< overload for detailed sensor information
    switch (sensor->getType()) {
        case Sensor::HYGROMETER:        // 0
            cout << *dynamic_cast<const Hygrometer*>(sensor);
            break;
        case Sensor::AIR_QUALITY:       // 1
            cout << *dynamic_cast<const AirQualitySensor*>(sensor);
            break;
        case Sensor::LUX_METER:         // 2
            cout << *dynamic_cast<const LuxMeterSensor*>(sensor);
            break;
        case Sensor::TEMPERATURE:       // 3
            cout << *dynamic_cast<const TemperatureSensor*>(sensor);
            break;
        case Sensor::CONTACT:           // 4
            cout << *dynamic_cast<const ContactSensor*>(sensor);
            break;
        case Sensor::THERMAL_CAMERA:    // 5
            cout << *dynamic_cast<const ThermalCamera*>(sensor);
            break;
        case Sensor::RGB_CAMERA:        // 6
            cout << *dynamic_cast<const RGBCamera*>(sensor);
            break;
        default:
            cout << "Unknown sensor type | Data: " << sensor->getSingleData();
            break;
    }
    
    cout << endl;
}

/**
 * @brief Test function for SensorCoordinator integration
 *
 * Shows the coordination between master sensors and global state.
 * Displays how master sensor readings match global coordinator values.
 *
 * @param db Reference to the SensorDatabase for testing
 */
void testSensorCoordination(SensorDatabase& db) {
    cout << "\n=== SENSOR COORDINATOR INTEGRATION TEST ===" << endl;
    
    // Find master sensors
    Sensor* masterTemp = db.findSensorById(TemperatureSensor::PRIMARY_TEMP_ID);
    Sensor* masterContact = db.findSensorById(ContactSensor::PRIMARY_CONTACT_ID);
    
    if (!masterTemp || !masterContact) {
        cout << "Error: Missing master sensors for coordination test!" << endl;
        return;
    }
    
    cout << "\nCOORDINATION STATUS:" << endl;
    cout << "==========================================" << endl;
    
    // Show temperature coordination
    cout << "TEMPERATURE COORDINATION:" << endl;
    cout << "  Master Sensor #" << TemperatureSensor::PRIMARY_TEMP_ID 
         << " reading: " << masterTemp->getSingleData() << "C" << endl;
    cout << "  Global Coordinator value: " << SensorCoordinator::getGlobalTemperature() << "C" << endl;
    cout << "  Status: " << (masterTemp->getSingleData() == SensorCoordinator::getGlobalTemperature() ? 
                             "SYNCHRONIZED" : "NOT SYNCHRONIZED") << endl;
    
    cout << "\nCONTACT/MOVEMENT COORDINATION:" << endl;
    cout << "  Master Sensor #" << ContactSensor::PRIMARY_CONTACT_ID 
         << " reading: " << masterContact->getSingleData() 
         << " (" << (masterContact->getSingleData() == 1 ? "OPEN" : "CLOSED") << ")" << endl;
    cout << "  Global Coordinator movement: " << (SensorCoordinator::isMovementDetected() ? 
                                                    "DETECTED" : "NONE") << endl;
    bool movementMatch = (masterContact->getSingleData() == 1) == SensorCoordinator::isMovementDetected();
    cout << "  Status: " << (movementMatch ? "SYNCHRONIZED" : "NOT SYNCHRONIZED") << endl;
    
    cout << "\n==========================================" << endl;
    cout << "Coordination test complete." << endl;
}

/**
 * @brief Pauses program execution until user presses Enter
 *
 * Provides a clean way to pause the program between operations,
 * allowing the user to read output before continuing.
 */
void pauseExecution() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}