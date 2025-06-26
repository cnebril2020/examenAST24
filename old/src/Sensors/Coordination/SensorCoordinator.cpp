#include "SensorCoordinator.h"
#include "../TemperatureSensor.h"
#include "../ContactSensor.h"
#include "../../Databases/SensorDatabase.h"
#include <iostream>

namespace SensorCoordinator {
    // Define the global variables
    int globalTemperature = 0;
    bool hasMovement = false;

    void setGlobalTemperature(int temp) {
        globalTemperature = temp;
    }

    void setMovementDetected(bool movement) {
        hasMovement = movement;
    }

    int getGlobalTemperature() {
        return globalTemperature;
    }

    bool isMovementDetected() {
        return hasMovement;
    }

    bool isTemperatureMaster(u_int32_t sensorId) {
        return sensorId == TemperatureSensor::PRIMARY_TEMP_ID;
    }

    bool isContactMaster(u_int32_t sensorId) {
        return sensorId == ContactSensor::PRIMARY_CONTACT_ID;
    }

    void initializeFromDatabase(SensorDatabase& db) {
        std::cout << "Initializing SensorCoordinator from persistent data..." << std::endl;
        
        // Load temperature from master sensor
        Sensor* masterTemp = db.findSensorById(TemperatureSensor::PRIMARY_TEMP_ID);
        if (masterTemp) {
            setGlobalTemperature(masterTemp->getSingleData());
            std::cout << "  - Loaded temperature: " << globalTemperature << "C" << std::endl;
        }
        
        // Load movement from master contact sensor
        Sensor* masterContact = db.findSensorById(ContactSensor::PRIMARY_CONTACT_ID);
        if (masterContact) {
            setMovementDetected(masterContact->getSingleData() == 1);
            std::cout << "  - Loaded movement: " << (hasMovement ? "YES" : "NO") << std::endl;
        }
        
        std::cout << "SensorCoordinator initialization complete." << std::endl;
    }
};