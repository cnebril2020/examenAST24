#include "Sensor.h"
#include "Hygrometer.h"
#include "AirQualitySensor.h"
#include "LuxMeterSensor.h"
#include "TemperatureSensor.h"
#include "ContactSensor.h"
#include "ThermalCamera.h"
#include "RGBCamera.h"
#include <iostream>

u_int32_t Sensor::sensorCount = 0;

u_int32_t Sensor::getSensorCount() {
    return sensorCount;
}

// Protected constructor - only derived classes can use it
Sensor::Sensor(u_int32_t sensorId, Type type) {
    setSensorId(sensorId); // Use the setter to validate
    this->type = type;
    // Initialize data array to zero
    for (size_t i = 0; i < MAX_DATA_SIZE; i++) {
        this->data[i] = 0;
    }
    this->sensorCount++;
}

Sensor::~Sensor() {
    this->sensorCount--;
}

// Implementation of the setSensorId method
void Sensor::setSensorId(const u_int32_t& newSensorId) {
    if (newSensorId < MIN_SENSOR_ID || newSensorId > MAX_SENSOR_ID) {
        throw std::invalid_argument(
            "Sensor ID must be 0 (primary) or between 10000 and 99999");
    }
    this->sensorId = newSensorId;
}

// Implementation of setFullData method
void Sensor::setFullData(const int* newData) {
    for (size_t i = 0; i < MAX_DATA_SIZE; i++) {
        this->data[i] = newData[i];
    }
}

bool Sensor::isPrimarySensor() const {
    return (sensorId == Hygrometer::PRIMARY_HYGRO_ID ||
            sensorId == AirQualitySensor::PRIMARY_AIR_QUALITY_ID ||
            sensorId == LuxMeterSensor::PRIMARY_LUX_ID ||
            sensorId == TemperatureSensor::PRIMARY_TEMP_ID ||
            sensorId == ContactSensor::PRIMARY_CONTACT_ID ||
            sensorId == ThermalCamera::PRIMARY_THERMAL_ID ||
            sensorId == RGBCamera::PRIMARY_RGB_ID);
}

// Helper method to distinguish sensor types
bool Sensor::isCamera() const {
    return (type == RGB_CAMERA || type == THERMAL_CAMERA);
}

// Overloading the << operator for Sensor
std::ostream& operator<<(std::ostream& os, const Sensor& sensor) {
    os << "Sensor #" << sensor.getSensorId() << " (";
    
    // Display sensor type
    switch(sensor.getType()) {
        case Sensor::TEMPERATURE:
            os << "Type: TEMPERATURE)" << std::endl;
            os << "Current reading: " << sensor.getSingleData() 
                << "°C" << std::endl;
            break;
        case Sensor::HYGROMETER:
            os << "Type: HUMIDITY)" << std::endl;
            os << "Current reading: " << sensor.getSingleData() 
                << "%" << std::endl;
            break;
        case Sensor::AIR_QUALITY:
            os << "Type: AIR_QUALITY)" << std::endl;
            os << "Current reading: " << sensor.getSingleData() 
                << " ppm" << std::endl;
            break;
        case Sensor::LUX_METER:
            os << "Type: LIGHT_LEVEL)" << std::endl;
            os << "Current reading: " << sensor.getSingleData() 
                << " lux" << std::endl;
            break;
        case Sensor::RGB_CAMERA:
            os << "Type: RGB_CAMERA)" << std::endl;
            os << "Image data: [" << Sensor::MAX_DATA_SIZE 
                << " pixels]" << std::endl;
            break;
        case Sensor::THERMAL_CAMERA:
            os << "Type: THERMAL_CAMERA)" << std::endl;
            os << "Thermal data: [" << Sensor::MAX_DATA_SIZE 
                << " temperature points]" << std::endl;
            break;
        case Sensor::CONTACT:
            os << "Type: CONTACT)" << std::endl;
            os << "Contact state: " 
                << (sensor.getSingleData() ? "OPEN" : "CLOSED") << std::endl;
            break;
        default:
            os << "Type: UNKNOWN)" << std::endl;
            break;
    }
    
    // Display ALL data for cameras (now manageable with 64 elements)
    if (sensor.isCamera()) {
        const int* fullData = sensor.getFullData();
        os << "Complete data: [";
        for (size_t i = 0; i < Sensor::MAX_DATA_SIZE; i++) {
            os << fullData[i];
            if (i < Sensor::MAX_DATA_SIZE - 1) os << ", ";
        }
        os << "]" << std::endl;
    }
    
    return os;
}

// Helper function to read sensor data with error handling
bool readSensorData(std::istream &is, u_int32_t &sensorId, char *typeStr) {
    if (!(is >> sensorId)) {
        std::cerr << "Error: Failed to read sensor ID" << std::endl;
        is.setstate(std::ios::failbit);
        return false;
    }
    
    if (!(is >> typeStr)) {
        std::cerr << "Error: Failed to read sensor type" << std::endl;
        is.setstate(std::ios::failbit);
        return false;
    }
    
    return true;
}

std::istream &operator>>(std::istream &is, Sensor &sensor) {
    // Check if stream is in a good state before proceeding
    if (!is) return is;

    u_int32_t sensorId;
    char typeStr[Sensor::MAX_DATA_SIZE];
    
    // Read all values with proper error handling
    if (!readSensorData(is, sensorId, typeStr)) {
        return is; // Error already reported and stream state set
    }
    
    // Validate and set all fields after successful reading
    try {
        sensor.setSensorId(sensorId);
        
        // Convert string to enum
        if (strcmp(typeStr, "TEMPERATURE") == 0) sensor.setType(Sensor::TEMPERATURE);
        else if (strcmp(typeStr, "HUMIDITY") == 0) sensor.setType(Sensor::HYGROMETER);
        else if (strcmp(typeStr, "AIR_QUALITY") == 0) sensor.setType(Sensor::AIR_QUALITY);
        else if (strcmp(typeStr, "LIGHT_LEVEL") == 0) sensor.setType(Sensor::LUX_METER);
        else if (strcmp(typeStr, "RGB_CAMERA") == 0) sensor.setType(Sensor::RGB_CAMERA);
        else if (strcmp(typeStr, "THERMAL_CAMERA") == 0) sensor.setType(Sensor::THERMAL_CAMERA);
        else if (strcmp(typeStr, "CONTACT") == 0) sensor.setType(Sensor::CONTACT);
        else {
            std::cerr << "Invalid sensor type value " << typeStr 
                << ". Setting to TEMPERATURE by default.\n";
            sensor.setType(Sensor::TEMPERATURE);
        }
        
        // Read sensor data based on type
        if (sensor.isCamera()) {
            // For cameras, read all 64 data points from the stream
            int cameraData[Sensor::MAX_DATA_SIZE];
            bool readSuccess = true;
            
            for (size_t i = 0; i < Sensor::MAX_DATA_SIZE; i++) {
                if (!(is >> cameraData[i])) {
                    std::cerr << "Failed to read camera data at position " << i 
                              << ". Filling remaining with zeros.\n";
                    // Fill remaining positions with zeros
                    for (size_t j = i; j < Sensor::MAX_DATA_SIZE; j++) {
                        cameraData[j] = 0;
                    }
                    readSuccess = false;
                    break;
                }
            }
            
            sensor.setFullData(cameraData);
            
            if (!readSuccess) {
                is.setstate(std::ios::failbit);
            }
            
        } else {
            // For regular sensors, read single value
            int value;
            if (is >> value) {
                sensor.setSingleData(value);
            } else {
                std::cerr << "Failed to read sensor value. Setting to 0.\n";
                sensor.setSingleData(0);
                is.setstate(std::ios::failbit);
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error setting sensor data: " << e.what() << std::endl;
        is.setstate(std::ios::failbit);
    }
    
    return is;
}

bool Sensor::operator<(const Sensor& other) const {
    return this->sensorId < other.sensorId;
}