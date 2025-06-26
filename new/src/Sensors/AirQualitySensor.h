#ifndef AIRQUALITYSENSOR_H
#define AIRQUALITYSENSOR_H

#include "Sensor.h"

using namespace std;

class AirQualitySensor : public Sensor {
public:
    static constexpr u_int32_t PRIMARY_AIR_QUALITY_ID = 20000;

    // Constructor - follows same pattern as User derived classes
    AirQualitySensor(u_int32_t sensorId);

    // Constructor 2: Primary sensor (ID=0)
    static AirQualitySensor* createPrimary();

    // Implement pure virtual method from Sensor
    void collectData() override;
    
    // Human-readable air quality interpretation
    const char* getAirQualityDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const AirQualitySensor& sensor);

private:
    // Private method to simulate reading from hardware
    int readAirQualityFromHardware();
};

#endif // AIRQUALITYSENSOR_H