#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <cstring>
#include <stdexcept>

using namespace std;

// Contains only data from the sensor. All operations are contained in 
// 'SensorDatabase' (future implementation).
class Sensor {
public:
    static constexpr u_int32_t MIN_SENSOR_ID = 10000;
    static constexpr u_int32_t MAX_SENSOR_ID = 99999;
    static constexpr size_t MAX_DATA_SIZE = 64; // Array size for camera data
    
    enum Type : u_int32_t {
        HYGROMETER = 0,
        AIR_QUALITY = 1,
        LUX_METER = 2,
        TEMPERATURE = 3,
        CONTACT = 4,
        THERMAL_CAMERA = 5,
        RGB_CAMERA = 6
    };
    
    // Abstract Class - Pure Virtual Destructor
    virtual ~Sensor() = 0;
    
    // Pure virtual method for collecting sensor data
    virtual void collectData() = 0;

    // Helper to distinguish sensor atributes
    bool isPrimarySensor() const;
    bool isCamera() const;

    // Getters
    u_int32_t getSensorId() const { return sensorId; }
    const int* getFullData() const { return data; } // Get full array (cameras)
    int getSingleData() const { return data[0]; }   // Get single value
    Type getType() const { return type; }
    static u_int32_t getSensorCount();
    
    // Setters
    void setSensorId(const u_int32_t& newSensorId);
    void setFullData(const int* newData);
    void setSingleData(int value) { data[0] = value; }
    void setType(Type type) { this->type = type; }
    
    // Overloaded Operators
    friend std::ostream& operator<<(std::ostream& os, const Sensor& sensor);
    friend std::istream& operator>>(std::istream& is, Sensor& sensor);
    bool operator<(const Sensor& other) const;

protected:
    // Constructor: Regular sensor with specific ID
    Sensor(u_int32_t sensorId, Type type);

    u_int32_t sensorId;
    int data[MAX_DATA_SIZE];
    Type type;

private:
    static u_int32_t sensorCount;
};

#endif // SENSOR_H
