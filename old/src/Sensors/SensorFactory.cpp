#include "SensorFactory.h"
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

// Convert Sensor to binary record
SensorRecord SensorFactory::sensorToRecord(const Sensor* sensor) {
    if (!sensor) {
        throw std::invalid_argument("Cannot convert null sensor to record");
    }
    
    SensorRecord record;
    record.sensorId = sensor->getSensorId();
    record.sensorType = static_cast<u_int32_t>(sensor->getType());
    
    // Copy all data (cameras use full array, others just [0])
    const int* sensorData = sensor->getFullData();
    for (size_t i = 0; i < Sensor::MAX_DATA_SIZE; i++) {
        record.data[i] = sensorData[i];
    }
    
    return record;
}

// Write Sensor directly to binary file
void SensorFactory::writeSensorToFile(std::ofstream& file, const Sensor* sensor) {
    if (!file.is_open()) {
        throw std::runtime_error("File is not open for writing");
    }
    if (!sensor) {
        throw std::invalid_argument("Cannot write null sensor to file");
    }
    
    // Convert sensor to binary record
    SensorRecord record = sensorToRecord(sensor);
    // Write record to file
    file.write(reinterpret_cast<char*>(&record), sizeof(SensorRecord));
}

// Helper function to check if sensor ID corresponds to a primary sensor
bool isPrimarySensorId(u_int32_t sensorId, Sensor::Type type) {
    switch (type) {
        case Sensor::HYGROMETER:
            return sensorId == Hygrometer::PRIMARY_HYGRO_ID;
        case Sensor::AIR_QUALITY:
            return sensorId == AirQualitySensor::PRIMARY_AIR_QUALITY_ID;
        case Sensor::LUX_METER:
            return sensorId == LuxMeterSensor::PRIMARY_LUX_ID;
        case Sensor::THERMAL_CAMERA:
            return sensorId == ThermalCamera::PRIMARY_THERMAL_ID;
        case Sensor::RGB_CAMERA:
            return sensorId == RGBCamera::PRIMARY_RGB_ID;
        case Sensor::TEMPERATURE:
            return sensorId == TemperatureSensor::PRIMARY_TEMP_ID;
        case Sensor::CONTACT:
            return sensorId == ContactSensor::PRIMARY_CONTACT_ID;
        default:
            return false;
    }
}

// Helper function to create primary sensors
Sensor* createPrimarySensor(Sensor::Type type) {
    switch (type) {
        case Sensor::HYGROMETER:
            return Hygrometer::createPrimary();
        case Sensor::AIR_QUALITY:
            return AirQualitySensor::createPrimary();
        case Sensor::LUX_METER:
            return LuxMeterSensor::createPrimary();
        case Sensor::THERMAL_CAMERA:
            return ThermalCamera::createPrimary();
        case Sensor::RGB_CAMERA:
            return RGBCamera::createPrimary();
        case Sensor::TEMPERATURE:
            return TemperatureSensor::createPrimary();
        case Sensor::CONTACT:
            return ContactSensor::createPrimary();
        default:
            return nullptr;
    }
}

// Helper function to create regular sensors
Sensor* createRegularSensor(Sensor::Type type, u_int32_t sensorId) {
    switch (type) {
        case Sensor::HYGROMETER:
            return new Hygrometer(sensorId);
        case Sensor::AIR_QUALITY:
            return new AirQualitySensor(sensorId);
        case Sensor::LUX_METER:
            return new LuxMeterSensor(sensorId);
        case Sensor::THERMAL_CAMERA:
            return new ThermalCamera(sensorId);
        case Sensor::RGB_CAMERA:
            return new RGBCamera(sensorId);
        case Sensor::TEMPERATURE:
            return new TemperatureSensor(sensorId);
        case Sensor::CONTACT:
            return new ContactSensor(sensorId);
        default:
            return nullptr;
    }
}

// Convert binary record to Sensor
Sensor* SensorFactory::recordToSensor(const SensorRecord& record) {
    try {
        Sensor* sensor = nullptr;
        
        // Create sensor based on type and whether it's primary
        bool isPrimary = isPrimarySensorId(record.sensorId, 
                static_cast<Sensor::Type>(record.sensorType));
        
        if (isPrimary) {
            sensor = createPrimarySensor(
                static_cast<Sensor::Type>(record.sensorType));
        } else {
            sensor = createRegularSensor(
                static_cast<Sensor::Type>(record.sensorType), record.sensorId);
        }
        
        if (sensor) {
            // Restore state and data from record
            sensor->setFullData(record.data);
        }
        
        return sensor;
    } catch (const std::exception& e) {
        std::cerr << "Error creating sensor from record: " << e.what() << endl;
        return nullptr;
    }
}

/**
 * Reads a sensor record from a binary file and creates a Sensor object
 * 
 * Uses validation to ensure data integrity
 * 
 * @param file Input file stream to read from (must be open in binary mode)
 * @return Pointer to newly created Sensor object or nullptr if:
 *         - End of file reached
 *         - Invalid/corrupted record detected
 *         - Memory allocation failed
 * @throws std::runtime_error if the file is not open
 * @note Caller is responsible for managing the returned object's memory
 */
Sensor* SensorFactory::readSensorFromFile(std::ifstream& file) {
    // Verify file is open before attempting to read
    if (!file.is_open()) {
        throw std::runtime_error("File is not open for reading");
    }
    
    SensorRecord record;
    // Read binary record from file
    if (file.read(reinterpret_cast<char*>(&record), sizeof(SensorRecord))) {
        // Multiple validations to ensure record integrity
        if (record.sensorId < Sensor::MIN_SENSOR_ID || 
            record.sensorId > Sensor::MAX_SENSOR_ID) {
            return nullptr; // Invalid record: sensor ID out of range
        }
        if (record.sensorType > 6) { // 7 types: 0-6
            return nullptr; // Invalid record: unrecognized sensor type
        }
        
        // All validations passed, convert record to Sensor object
        return recordToSensor(record);
    }
    
    // End of file or read error occurred
    return nullptr;
}