#include <iostream>
#include <cstdint>
#include "../TemperatureSensor.h"
#include "../ContactSensor.h"
#include "../ThermalCamera.h"
#include "../RGBCamera.h"
#include "SensorCoordinator.h"

using namespace std;

int main() {
    cout << "=== SIMPLE COORDINATION TEST ===" << endl;

    // Create sensors
    TemperatureSensor* masterTemp = TemperatureSensor::createPrimary();
    TemperatureSensor normalTemp(10001);
    ContactSensor* masterContact = ContactSensor::createPrimary();
    ContactSensor normalContact(20001);
    
    // Create cameras
    ThermalCamera* thermalCam = ThermalCamera::createPrimary();
    RGBCamera* rgbCam = RGBCamera::createPrimary();

    cout << "\n1. Initial state:" << endl;
    SensorCoordinator::printCurrentState();

    cout << "\n2. Testing master validation:" << endl;
    cout << "Temp #" << TemperatureSensor::PRIMARY_TEMP_ID << " is master: " 
         << (SensorCoordinator::isTemperatureMaster(TemperatureSensor::PRIMARY_TEMP_ID) ? "YES" : "NO") << endl;
    cout << "Temp #10001 is master: " 
         << (SensorCoordinator::isTemperatureMaster(10001) ? "YES" : "NO") << endl;
    cout << "Contact #" << ContactSensor::PRIMARY_CONTACT_ID << " is master: " 
         << (SensorCoordinator::isContactMaster(ContactSensor::PRIMARY_CONTACT_ID) ? "YES" : "NO") << endl;

    cout << "\n3. Collecting data:" << endl;
    masterTemp->collectData();      // This updates global state
    normalTemp.collectData();       // This doesn't
    masterContact->collectData();   // This updates global state
    normalContact.collectData();    // This doesn't

    cout << "\n4. Final state:" << endl;
    SensorCoordinator::printCurrentState();

    cout << "\n5. Camera data (uses coordinated data):" << endl;
    thermalCam->collectData();
    rgbCam->collectData();

    cout << "\n6. Individual vs Coordinated comparison:" << endl;
    cout << "Master temp: " << masterTemp->getSingleData() << "°C" << endl;
    cout << "Normal temp: " << normalTemp.getSingleData() << "°C" << endl;
    cout << "Global temp: " << SensorCoordinator::getGlobalTemperature() << "°C" << endl;

    // Cleanup
    delete masterTemp;
    delete masterContact;
    delete thermalCam;
    delete rgbCam;

    cout << "\n=== TEST COMPLETED ===" << endl;
    return 0;
}

// To compile:
// g++ -std=c++11 -I ../../ -o sensorCoordinatorTest sensorCoordinatorTest.cpp ../Sensor.cpp ../TemperatureSensor.cpp ../ContactSensor.cpp ../ThermalCamera.cpp ../RGBCamera.cpp SensorCoordinator.cpp