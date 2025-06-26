#ifndef SENSORCOORDINATOR_H
#define SENSORCOORDINATOR_H

#include <sys/types.h>

class SensorDatabase;

namespace SensorCoordinator {
    // (Setters) Data update methods - only called by master sensors
    void setGlobalTemperature(int temp);
    void setMovementDetected(bool movement);
    
    // (Getters) Data access methods - used by cameras
    int getGlobalTemperature();
    bool isMovementDetected();
    
    // Master validation methods
    bool isTemperatureMaster(u_int32_t sensorId);
    bool isContactMaster(u_int32_t sensorId);

    // Initialization method
    void initializeFromDatabase(SensorDatabase& db);

    // Global shared state - from master sensors 
    extern int globalTemperature;   // From master TemperatureSensor (ID=40000)
    extern bool hasMovement;        // From master ContactSensor (ID=50000)
}

#endif // SENSORCOORDINATOR_H