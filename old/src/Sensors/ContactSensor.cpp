#include "ContactSensor.h"
#include "Coordination/SensorCoordinator.h" 
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor - calls parent constructor with type
ContactSensor::ContactSensor(u_int32_t sensorId) 
    : Sensor(sensorId, Type::CONTACT) {
    collectData();
}

ContactSensor* ContactSensor::createPrimary() {
    cout << "[SYSTEM] Loading PRIMARY ContactSensor (ID=" 
         << PRIMARY_CONTACT_ID << ")" << endl;
    return new ContactSensor(PRIMARY_CONTACT_ID);
}

// Implementation of pure virtual method collectData
void ContactSensor::collectData() {
    readContactFromHardware();
}

// Helper method to interpret contact state
bool ContactSensor::isContactOpen() const {
    return getSingleData() == 1;
}

// Human-readable contact state description
const char* ContactSensor::getContactStateDescription() const {
    return isContactOpen() ? "OPEN" : "CLOSED";
}

// Private method to simulate reading from hardware
int ContactSensor::readContactFromHardware() {
    // Simulate contact sensor reading
    // 0 = CLOSED (no movement)
    // 1 = OPEN (movement detected)
    
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    
    // Generate realistic contact patterns
    // 70% chance closed, 30% chance open
    int contactReading = (rand() % 100 < 30) ? 1 : 0;
    
    // Set the sensor data
    setSingleData(contactReading);

    if (SensorCoordinator::isContactMaster(getSensorId())) {
        SensorCoordinator::setMovementDetected(contactReading == 1);
    }
    
    return contactReading;
}

std::ostream& operator<<(std::ostream& os, const ContactSensor& sensor) {
    os << "Contact: " << sensor.getSingleData();
    os << " (" << sensor.getContactStateDescription();
    os << " - " << (sensor.isContactOpen() ? "OPEN" : "CLOSED") << ")";
    return os;
}