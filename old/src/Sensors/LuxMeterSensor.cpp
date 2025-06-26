#include "LuxMeterSensor.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor - calls parent constructor with type
LuxMeterSensor::LuxMeterSensor(u_int32_t sensorId) 
    : Sensor(sensorId, Type::LUX_METER) {
    collectData();
}

LuxMeterSensor* LuxMeterSensor::createPrimary() {
    cout << "[SYSTEM] Loading PRIMARY LuxMeterSensor (ID=" 
         << PRIMARY_LUX_ID << ")" << endl;
    return new LuxMeterSensor(PRIMARY_LUX_ID);
}

// Implementation of pure virtual method collectData
void LuxMeterSensor::collectData() {
    readLuxFromHardware();
}

// Human-readable light level interpretation
const char* LuxMeterSensor::getLightLevelDescription() const {
    int lux = getSingleData();
    
    if (lux < 1) {
        return "DARK";
    } else if (lux <= 10) {
        return "VERY DIM";
    } else if (lux <= 50) {
        return "DIM";
    } else if (lux <= 200) {
        return "LOW LIGHT";
    } else if (lux <= 500) {
        return "NORMAL";
    } else if (lux <= 1000) {
        return "BRIGHT";
    } else if (lux <= 10000) {
        return "VERY BRIGHT";
    } else {
        return "DAYLIGHT";
    }
}

// Private method to simulate reading from hardware
int LuxMeterSensor::readLuxFromHardware() {
    // Simulate lux reading (light intensity)
    // Common lux values:
    // - Moonlight: 0.1-1 lux
    // - Candle: 10-15 lux
    // - Street light: 15-50 lux
    // - Living room: 50-200 lux
    // - Office lighting: 200-500 lux
    // - Bright office: 500-1000 lux
    // - Daylight (indoors): 1000-5000 lux
    // - Direct sunlight: 10000-100000 lux
    
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    
    // Generate realistic lux values based on time simulation
    // Simulate different lighting conditions with weighted probabilities
    int scenario = rand() % 100;
    int luxValue;
    
    if (scenario < 5) {
        // 5% - Very dark (night, power outage)
        luxValue = rand() % 10; // 0-9 lux
    } else if (scenario < 15) {
        // 10% - Dim lighting (emergency lighting, dawn/dusk)
        luxValue = 10 + (rand() % 40); // 10-49 lux
    } else if (scenario < 35) {
        // 20% - Low light (evening indoor)
        luxValue = 50 + (rand() % 150); // 50-199 lux
    } else if (scenario < 70) {
        // 35% - Normal indoor lighting
        luxValue = 200 + (rand() % 300); // 200-499 lux
    } else if (scenario < 90) {
        // 20% - Bright indoor lighting
        luxValue = 500 + (rand() % 500); // 500-999 lux
    } else if (scenario < 98) {
        // 8% - Very bright (near windows, bright office)
        luxValue = 1000 + (rand() % 4000); // 1000-4999 lux
    } else {
        // 2% - Daylight conditions
        luxValue = 5000 + (rand() % 15000); // 5000-19999 lux
    }
    
    // Add small random variance for realism
    int variance = (rand() % 21) - 10; // -10 to +10
    luxValue += variance;
    
    // Ensure lux is within valid range
    if (luxValue < 0) luxValue = 0;
    if (luxValue > 100000) luxValue = 100000;
    
    // Set the sensor data
    setSingleData(luxValue);
    
    return luxValue;
}

std::ostream& operator<<(std::ostream& os, const LuxMeterSensor& sensor) {
    os << "Light: " << sensor.getSingleData() << " lux";
    os << " (" << sensor.getLightLevelDescription() << ")";
    return os;
}