#ifndef HYGROMETER_H
#define HYGROMETER_H

#include "Sensor.h"

using namespace std;

class Hygrometer : public Sensor {
public:
    static constexpr u_int32_t PRIMARY_HYGRO_ID = 10000;

    // Constructor - follows same pattern as other derived classes
    Hygrometer(u_int32_t sensorId);

    // Constructor 2: Primary sensor (ID=0)
    static Hygrometer* createPrimary();

    // Implement pure virtual method from Sensor
    void collectData() override;
    
    // Helper methods to interpret humidity data
    const char* getHumidityDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const Hygrometer& sensor);

private:
    // Private method to simulate reading from hardware
    int readHumidityFromHardware();
};

#endif // HYGROMETER_H