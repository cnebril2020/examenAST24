#ifndef CONTACTSENSOR_H
#define CONTACTSENSOR_H

#include "Sensor.h"

using namespace std;

class ContactSensor : public Sensor {
public:
    static constexpr u_int32_t PRIMARY_CONTACT_ID = 50000;

    // Constructor - follows same pattern as other derived classes
    ContactSensor(u_int32_t sensorId);

    // Constructor 2: Primary sensor (ID=0)
    static ContactSensor* createPrimary();

    // Implement pure virtual method from Sensor
    void collectData() override;
    
    // Helper method to interpret contact state
    const char* getContactStateDescription() const;
    bool isContactOpen() const;

    friend std::ostream& operator<<(std::ostream& os, const ContactSensor& sensor);

private:
    // Private method to simulate reading from hardware
    int readContactFromHardware();
};

#endif // CONTACTSENSOR_H