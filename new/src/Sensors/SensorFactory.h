#ifndef SENSOR_FACTORY_H
#define SENSOR_FACTORY_H

#include "Sensor.h"
#include "TemperatureSensor.h"
#include "Hygrometer.h"
#include "AirQualitySensor.h"
#include "LuxMeterSensor.h"
#include "RGBCamera.h"
#include "ThermalCamera.h"
#include "ContactSensor.h"
#include <fstream>
#include <iostream>

// Structure representing a binary sensor record
struct SensorRecord {
    u_int32_t sensorId;
    u_int32_t sensorType;
    int data[Sensor::MAX_DATA_SIZE]; // Full data array (cameras use all, others use [0])
};

class SensorFactory {
public:
    // Binary record conversion methods
    static SensorRecord sensorToRecord(const Sensor* sensor);
    static Sensor* recordToSensor(const SensorRecord& record);
    
    // Methods for direct binary file access
    static void writeSensorToFile(std::ofstream& file, const Sensor* sensor);
    static Sensor* readSensorFromFile(std::ifstream& file);
};

#endif // SENSOR_FACTORY_H