#include "TemperatureSensor.h"
#include "Coordination/SensorCoordinator.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor - calls parent constructor with type and specific id
TemperatureSensor::TemperatureSensor(u_int32_t sensorId) 
    : Sensor(sensorId, Type::TEMPERATURE) {
    collectData();
}

TemperatureSensor* TemperatureSensor::createPrimary() {
    cout << "[SYSTEM] Loading PRIMARY TemperatureSensor (ID=" 
         << PRIMARY_TEMP_ID << ")" << endl;
    return new TemperatureSensor(PRIMARY_TEMP_ID);
}

// Implementation of pure virtual method collectData
void TemperatureSensor::collectData() {
    readTemperatureFromHardware();
}

// Human-readable temperature interpretation
const char* TemperatureSensor::getTemperatureDescription() const {
    int temp = getSingleData();
    
    if (temp < 0) {
        return "FREEZING";
    } else if (temp <= 10) {
        return "VERY COLD";
    } else if (temp <= 16) {
        return "COLD";
    } else if (temp <= 18) {
        return "COOL";
    } else if (temp <= 25) {
        return "COMFORTABLE";
    } else if (temp <= 30) {
        return "WARM";
    } else if (temp <= 35) {
        return "HOT";
    } else {
        return "VERY HOT";
    }
}

// Private method to simulate reading from hardware
int TemperatureSensor::readTemperatureFromHardware() {
    // Simulate temperature reading in Celsius
    // Typical indoor temperature ranges: 16-30°C
    
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    
    // Base comfortable temperature (18-25°C range)
    int baseTemperature = 18 + (rand() % 8); // 18-25°C
    
    // Add occasional extreme variations
    int extremeChance = rand() % 100;
    if (extremeChance < 10) {
        // 10% chance of cold (5-17°C)
        baseTemperature = 5 + (rand() % 13);
    } else if (extremeChance < 20) {
        // 10% chance of hot (26-40°C)
        baseTemperature = 26 + (rand() % 15);
    }

    // Add small random variation (-2 to +2)
    int variance = (rand() % 5) - 2;
    int temperatureReading = baseTemperature + variance;
    
    // Set the sensor data
    setSingleData(temperatureReading);
    
    // COORDINATION: Only master updates global state
    if (SensorCoordinator::isTemperatureMaster(getSensorId())) {
        SensorCoordinator::setGlobalTemperature(temperatureReading);
    }

    return temperatureReading;
}

std::ostream &operator<<(std::ostream &os, const TemperatureSensor &sensor) {
    os << "Temperature: " << sensor.getSingleData() << "°C";
    os << " (" << sensor.getTemperatureDescription() << ")";
    return os;
}
