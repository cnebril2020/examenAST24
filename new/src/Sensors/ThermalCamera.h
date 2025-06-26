#ifndef THERMALCAMERA_H
#define THERMALCAMERA_H

#include "Sensor.h"

using namespace std;

class ThermalCamera : public Sensor {
public:
    static constexpr u_int32_t PRIMARY_THERMAL_ID = 60000;

    // Constructor - follows same pattern as other derived classes
    ThermalCamera(u_int32_t sensorId);

    // Constructor 2: Primary sensor (ID=0)
    static ThermalCamera* createPrimary();
    
    // Implement pure virtual method from Sensor
    void collectData() override;
    
    // Helper methods to interpret thermal data
    const char* getThermalDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const ThermalCamera& sensor);

private:
    // Private method to simulate reading from hardware
    void readThermalDataFromHardware();
};

#endif // 