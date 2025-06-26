#ifndef LUXMETERSENSOR_H
#define LUXMETERSENSOR_H

#include "Sensor.h"

using namespace std;

class LuxMeterSensor : public Sensor {
public:
    static constexpr u_int32_t PRIMARY_LUX_ID = 30000;

    // Constructor - follows same pattern as other derived classes
    LuxMeterSensor(u_int32_t sensorId);

    // Constructor 2: Primary sensor (ID=0)
    static LuxMeterSensor* createPrimary();

    // Implement pure virtual method from Sensor
    void collectData() override;
    
    // Helper methods to interpret light level data
    const char* getLightLevelDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const LuxMeterSensor& sensor);

private:
    // Private method to simulate reading from hardware
    int readLuxFromHardware();
};

#endif // LUXMETERSENSOR_H