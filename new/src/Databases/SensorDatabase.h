#ifndef SENSORDATABASE_H
#define SENSORDATABASE_H

#include <vector>
#include <string>
#include "../Sensors/Sensor.h"
#include "Database.h"

class Sensor;

class SensorDatabase : public Database {
public:
    SensorDatabase(const char* filename = "sensors.dat");
    ~SensorDatabase();

    // File Management (inherited from Database)
    bool loadFromFile(const char* filename) override;
    bool saveToFile(const char* filename) override;
    bool clearFile(const char* filename) override;

    // Sensor Management
    bool addSensor(Sensor* sensor);
    bool updateSensor(Sensor* sensor);
    bool removeSensor(Sensor* sensor);
    Sensor* findSensorById(u_int32_t sensorId) const;

    // Single getter 
    std::vector<Sensor*> getAllSensors() const;

private:
    std::vector<Sensor*> sensors;
};

#endif // SENSORDATABASE_H