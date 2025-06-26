#include "Hygrometer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor - calls parent constructor with type
Hygrometer::Hygrometer(u_int32_t sensorId) 
    : Sensor(sensorId, Type::HYGROMETER) {
    collectData();
}

Hygrometer* Hygrometer::createPrimary() {
    cout << "[SYSTEM] Loading PRIMARY Hygrometer Sensor (ID=" << PRIMARY_HYGRO_ID << ")" << endl;
    return new Hygrometer(PRIMARY_HYGRO_ID);
}

// Implementation of pure virtual method collectData
void Hygrometer::collectData() {
    readHumidityFromHardware();
}

// Human-readable humidity interpretation
const char* Hygrometer::getHumidityDescription() const {
    int humidity = getSingleData();
    
    if (humidity < 30) {
        return "TOO DRY";
    } else if (humidity <= 35) {
        return "DRY";
    } else if (humidity <= 60) {
        return "OPTIMAL";
    } else if (humidity <= 70) {
        return "HUMID";
    } else {
        return "TOO HUMID";
    }
}

// Private method to simulate reading from hardware
int Hygrometer::readHumidityFromHardware() {
    // Simulate humidity reading in percentage (0-100% RH)
    // Typical indoor humidity ranges:
    // - Too dry: 0-30%
    // - Dry: 30-35%
    // - Optimal: 35-60%
    // - Humid: 60-70%
    // - Too humid: 70-100%
    
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    
    // Generate realistic humidity values with seasonal variation
    // Base humidity around 45% (comfortable indoor level)
    int baseHumidity = 45;
    
    // Add realistic variance (-25 to +35 for seasonal changes)
    int variance = (rand() % 61) - 25; // -25 to +35
    int finalHumidity = baseHumidity + variance;
    
    // Ensure humidity is within valid range (0-100%)
    if (finalHumidity < 0) finalHumidity = 0;
    if (finalHumidity > 100) finalHumidity = 100;
    
    // Set the sensor data
    setSingleData(finalHumidity);
    
    return finalHumidity;
}

std::ostream& operator<<(std::ostream& os, const Hygrometer& sensor) {
    os << "Humidity: " << sensor.getSingleData() << "%";
    os << " (" << sensor.getHumidityDescription() << ")";
    return os;
}