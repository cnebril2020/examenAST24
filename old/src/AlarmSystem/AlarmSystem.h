#ifndef ALARMSYSTEM_H
#define ALARMSYSTEM_H

#include "../Sensors/RGBCamera.h"
#include "../Databases/SensorDatabase.h"
#include <vector>

class AlarmSystem {
public:
    // Constructor - requires reference to sensor database
    explicit AlarmSystem(SensorDatabase& sensorDb);
    
    // Destructor
    ~AlarmSystem() = default;

    // Single method - checks status and dumps cameras if activity detected
    bool checkAlarm();

private:
    SensorDatabase& database;
    
    // Internal method for RGB cameras dump
    void dumpRGBCameras();
    
    // Finds all RGB cameras in the system
    std::vector<RGBCamera*> findRGBCameras();
};

#endif // ALARMSYSTEM_H