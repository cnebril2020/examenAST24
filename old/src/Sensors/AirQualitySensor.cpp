#include "AirQualitySensor.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor - calls parent constructor with type
AirQualitySensor::AirQualitySensor(u_int32_t sensorId) 
    : Sensor(sensorId, Type::AIR_QUALITY) {
    collectData();
}

AirQualitySensor* AirQualitySensor::createPrimary() {
    cout << "[SYSTEM] Loading PRIMARY AirQualitySensor (ID=" 
         << PRIMARY_AIR_QUALITY_ID << ")" << endl;
    return new AirQualitySensor(PRIMARY_AIR_QUALITY_ID);
}

// Implementation of pure virtual method collectData
void AirQualitySensor::collectData() {
    readAirQualityFromHardware();
}

// Human-readable air quality interpretation
const char* AirQualitySensor::getAirQualityDescription() const {
    int reading = getSingleData();
    
    if (reading <= 50) {
        return "EXCELLENT";
    } else if (reading <= 100) {
        return "GOOD";
    } else if (reading <= 150) {
        return "MODERATE";
    } else if (reading <= 200) {
        return "POOR";
    } else if (reading <= 300) {
        return "UNHEALTHY";
    } else {
        return "HAZARDOUS";
    }
}

// Private method to simulate reading from hardware
int AirQualitySensor::readAirQualityFromHardware() {
    // Simulate air quality reading in ppm (parts per million)
    // Good air quality: 0-50 ppm
    // Moderate: 51-100 ppm  
    // Unhealthy: 101-300 ppm
    // Hazardous: 301+ ppm
    
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    
    // Generate realistic air quality values (0-400 ppm)
    int baseReading = rand() % 401; // 0-400
    
    // Add some realistic variance
    int variance = (rand() % 21) - 10; // -10 to +10
    int finalReading = baseReading + variance;
    
    // Ensure reading is within valid range
    if (finalReading < 0) finalReading = 0;
    if (finalReading > 500) finalReading = 500;
    
    // Set the sensor data
    setSingleData(finalReading);
    
    return finalReading;
}

std::ostream& operator<<(std::ostream& os, const AirQualitySensor& sensor) {
    os << "Air Quality: " << sensor.getSingleData() << " ppm";
    os << " (" << sensor.getAirQualityDescription() << ")";
    return os;
}