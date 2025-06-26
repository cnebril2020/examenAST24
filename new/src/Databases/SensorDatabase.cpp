#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include "SensorDatabase.h"
#include "../Sensors/Sensor.h"
#include "../Sensors/TemperatureSensor.h"
#include "../Sensors/Hygrometer.h"
#include "../Sensors/AirQualitySensor.h"
#include "../Sensors/LuxMeterSensor.h"
#include "../Sensors/RGBCamera.h"
#include "../Sensors/ThermalCamera.h"
#include "../Sensors/ContactSensor.h"
#include "../Sensors/SensorFactory.h"

class Sensor;
class TemperatureSensor;
class Hygrometer;
class AirQualitySensor;
class LuxMeterSensor;
class RGBCamera;
class ThermalCamera;
class ContactSensor;
class SensorFactory;

using namespace std;

// Constructor
SensorDatabase::SensorDatabase(const char* inputFilename) {
    // Copy the input filename to the member variable with bounds checking
    strncpy(this->filename, inputFilename, MAX_STR - 1);
    this->filename[MAX_STR - 1] = '\0'; // Ensure null termination
    
    // Make sure the filename has .dat extension
    size_t len = strlen(this->filename);
    if (len < 4 || strcmp(this->filename + len - 4, ".dat") != 0) {
        // If filename doesn't end with .dat, append it (if there's space)
        if (len + 4 < MAX_STR) {
            strcat(this->filename, ".dat");
        } else {
            cerr << "Warning: Filename too long to append .dat extension\n";
        }
    }

    // First, load existing sensors from file if it exists
    loadFromFile(this->filename);

    // After loading, check if primary sensors exist and create them if needed
    // Use the actual constants from each sensor class (with their specific names)
    u_int32_t primaryIds[] = {
        Hygrometer::PRIMARY_HYGRO_ID,             // 10000
        AirQualitySensor::PRIMARY_AIR_QUALITY_ID, // 20000
        LuxMeterSensor::PRIMARY_LUX_ID,           // 30000
        TemperatureSensor::PRIMARY_TEMP_ID,       // 40000
        ContactSensor::PRIMARY_CONTACT_ID,        // 50000
        ThermalCamera::PRIMARY_THERMAL_ID,        // 60000
        RGBCamera::PRIMARY_RGB_ID                 // 70000
    };
    
    for (u_int32_t id : primaryIds) {
        // Only create primary sensor if it doesn't exist
        if (findSensorById(id) == nullptr) {
            Sensor* primarySensor = nullptr;
            
            // Use the actual constants with their specific names
            if (id == Hygrometer::PRIMARY_HYGRO_ID) {
                primarySensor = Hygrometer::createPrimary();
            } else if (id == AirQualitySensor::PRIMARY_AIR_QUALITY_ID) {
                primarySensor = AirQualitySensor::createPrimary();
            } else if (id == LuxMeterSensor::PRIMARY_LUX_ID) {
                primarySensor = LuxMeterSensor::createPrimary();
            } else if (id == ThermalCamera::PRIMARY_THERMAL_ID) {
                primarySensor = ThermalCamera::createPrimary();
            } else if (id == RGBCamera::PRIMARY_RGB_ID) {
                primarySensor = RGBCamera::createPrimary();
            } else if (id == TemperatureSensor::PRIMARY_TEMP_ID) {
                primarySensor = TemperatureSensor::createPrimary();
            } else if (id == ContactSensor::PRIMARY_CONTACT_ID) {
                primarySensor = ContactSensor::createPrimary();
            }
            
            if (primarySensor) {
                this->sensors.push_back(primarySensor);
            }
        }
    }
}

// Destructor
SensorDatabase::~SensorDatabase() {
    // Save to file before destroying
    saveToFile(this->filename);
    
    // Free memory for all sensors
    for (auto sensor : sensors) {
        delete sensor;
    }

    sensors.clear();
}

bool SensorDatabase::loadFromFile(const char* filename) {
    // Open file in binary mode
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Warning: Could not open file '" 
            << filename << "' for reading." << endl;
        return false;
    }

    // Read records until end of file
    Sensor* sensor = nullptr;
    while ((sensor = SensorFactory::readSensorFromFile(file)) != nullptr) {
        // Check for duplicates by ID
        if (findSensorById(sensor->getSensorId()) == nullptr) {
            sensors.push_back(sensor);
        } else {
            cerr << "Warning: Duplicate sensor with ID " 
                 << sensor->getSensorId() << " ignored" << endl;
            delete sensor; // Clean up if duplicate
        }
    }

    file.close();
    return true;
}

bool SensorDatabase::saveToFile(const char* filename) {
    // Open file in binary mode with truncation
    ofstream file(filename, ios::out | ios::binary | ios::trunc);
    if (!file.is_open()) {
        throw runtime_error("Could not open file for writing.");
    }

    // Write each sensor as a binary record
    for (auto sensor : sensors) {
        SensorFactory::writeSensorToFile(file, sensor);
    }

    file.close();
    return true;
}

bool SensorDatabase::clearFile(const char* filename) {
    // First, identify and preserve primary sensors (like default admin preservation)
    vector<Sensor*> primarySensors;
    
    for (auto it = sensors.begin(); it != sensors.end();) {
        if ((*it)->isPrimarySensor()) {
            primarySensors.push_back(*it);
            it = sensors.erase(it);
        } else {
            delete *it;  // Free memory
            it = sensors.erase(it);
        }
    }
    
    // Put primary sensors back
    sensors = primarySensors;
    
    // Open file in binary mode with truncation
    ofstream file(filename, ios::out | ios::binary | ios::trunc);
    if (!file.is_open()) {
        throw runtime_error("Could not open file for clearing.");
    }
    
    // Write the primary sensors back to the file
    for (auto sensor : sensors) {
        SensorFactory::writeSensorToFile(file, sensor);
    }
    
    file.close();
    return true;
}

bool SensorDatabase::addSensor(Sensor* sensor) {
    if (!sensor) {
        throw invalid_argument("Cannot add null sensor");
    }
    
    // Check if a sensor with this ID already exists
    if (findSensorById(sensor->getSensorId()) != nullptr) {
        return false; // Sensor already exists
    }
    
    // Validate sensor ID range
    if (sensor->getSensorId() < Sensor::MIN_SENSOR_ID || 
        sensor->getSensorId() > Sensor::MAX_SENSOR_ID) {
        throw invalid_argument("Sensor ID must be between " + 
                              to_string(Sensor::MIN_SENSOR_ID) + " and " + 
                              to_string(Sensor::MAX_SENSOR_ID));
    }
    
    // Add the sensor to the vector
    sensors.push_back(sensor);
    return true;
}

bool SensorDatabase::updateSensor(Sensor* sensor) {
    if (!sensor) {
        throw invalid_argument("Cannot update null sensor");
    }
    
    // Find the sensor by ID
    auto it = find_if(sensors.begin(), sensors.end(),
                      [sensor](const Sensor* s) {
                          return s->getSensorId() == sensor->getSensorId();
                      });
    
    if (it == sensors.end()) {
        return false; // Sensor not found
    }
    
    // Replace the sensor (keeping old pointer for deletion)
    *it = sensor;
    
    return true;
}

/**
 * @brief Remove a sensor from the database
 * 
 * @param sensor Pointer to the sensor to be removed
 * @return true if sensor was successfully removed
 * @throws runtime_error if:
 *         - sensor is null
 *         - sensor is a primary sensor (cannot be removed)
 *         - sensor is not found in the database
 */
bool SensorDatabase::removeSensor(Sensor* sensor) {
    if (!sensor) {
        throw invalid_argument("Cannot remove null sensor");
    }
    
    // Don't allow removing primary sensors (like admin #10000 protection)
    if (sensor->isPrimarySensor()) {
        throw runtime_error("Cannot remove primary sensor with ID " + 
                           to_string(sensor->getSensorId()));
    }
    
    // Find sensor in vector
    auto it = find(sensors.begin(), sensors.end(), sensor);
    if (it == sensors.end()) {
        throw runtime_error("Sensor not found in database");
    }
    
    delete *it;

    // Remove sensor from vector
    sensors.erase(it);
    
    // Save changes to file
    saveToFile(filename);
    
    return true;
}

Sensor* SensorDatabase::findSensorById(u_int32_t sensorId) const {
    for (auto sensor : sensors) {
        if (sensor->getSensorId() == sensorId) {
            return sensor;
        }
    }
    return nullptr; // Sensor not found
}

std::vector<Sensor*> SensorDatabase::getAllSensors() const {
    return sensors;
}