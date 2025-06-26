#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Sensor.h"

using namespace std;

class TemperatureSensor : public Sensor {
public:
    static constexpr u_int32_t PRIMARY_TEMP_ID = 40000;

    // Constructor - follows same pattern as other derived classes
    TemperatureSensor(u_int32_t sensorId);

    // Constructor 2: Primary sensor (ID=0)
    static TemperatureSensor* createPrimary();

    // Implement pure virtual method from Sensor
    void collectData() override;
    
    // Helper methods to interpret temperature data
    const char* getTemperatureDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const TemperatureSensor& sensor);

    private:
    // Private method to simulate reading from hardware
    int readTemperatureFromHardware();
};

#endif // TEMPERATURESENSOR_H