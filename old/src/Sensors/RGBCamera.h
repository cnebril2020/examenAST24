#ifndef RGBCAMERA_H
#define RGBCAMERA_H

#include "Sensor.h"

using namespace std;

class RGBCamera : public Sensor {
public:
    static constexpr u_int32_t PRIMARY_RGB_ID = 70000;

    // Constructor - follows same pattern as other derived classes
    RGBCamera(u_int32_t sensorId);

    // Constructor 2: Primary sensor (ID=0)
    static RGBCamera* createPrimary();

    // Implement pure virtual method from Sensor
    void collectData() override;
    
    // Helper method to interpret RGB data
    const char* getImageQualityDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const RGBCamera& sensor);

private:
    // Private method to simulate reading from hardware
    void readRGBDataFromHardware();
};

#endif // RGBCAMERA_H