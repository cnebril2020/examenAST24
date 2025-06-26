#include "ThermalCamera.h"
#include "Coordination/SensorCoordinator.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>

using namespace std;

// Constructor - calls parent constructor with type
ThermalCamera::ThermalCamera(u_int32_t sensorId) 
    : Sensor(sensorId, Type::THERMAL_CAMERA) {
    collectData();
}

ThermalCamera* ThermalCamera::createPrimary() {
    cout << "[SYSTEM] Loading PRIMARY ThermalCamera (ID=" 
         << PRIMARY_THERMAL_ID << ")" << endl;
    return new ThermalCamera(PRIMARY_THERMAL_ID);
}

// Implementation of pure virtual method collectData
void ThermalCamera::collectData() {
    readThermalDataFromHardware();
}

const char* ThermalCamera::getThermalDescription() const {
    // COORDINATION: Use global temperature instead of calculated average
    int coordTemp = SensorCoordinator::getGlobalTemperature();
    
    if (coordTemp < 0) {
        return "THERMAL: FREEZING ZONES";
    } else if (coordTemp <= 10) {
        return "THERMAL: VERY COLD AREAS";
    } else if (coordTemp <= 16) {
        return "THERMAL: COLD REGIONS";
    } else if (coordTemp <= 18) {
        return "THERMAL: COOL ZONES";
    } else if (coordTemp <= 25) {
        return "THERMAL: COMFORTABLE AREAS";
    } else if (coordTemp <= 30) {
        return "THERMAL: WARM REGIONS";
    } else if (coordTemp <= 35) {
        return "THERMAL: HOT ZONES";
    } else {
        return "THERMAL: VERY HOT AREAS";
    }
}

void ThermalCamera::readThermalDataFromHardware() {
    for (size_t i = 0; i < MAX_DATA_SIZE; i++) {
        data[i] = 0;
    }
    
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    
    int thermalData[MAX_DATA_SIZE];
    
    // COORDINATION: Use global temperature instead of random scenarios
    int baseTemp = SensorCoordinator::getGlobalTemperature();
    
    // Generate temperature points around the coordinated base temperature
    for (size_t i = 0; i < MAX_DATA_SIZE; i++) {
        int variation = (rand() % 11) - 5; // -5 to +5°C around base
        int tempPoint = baseTemp + variation;
        
        // Ensure realistic range
        if (tempPoint < -10) tempPoint = -10;
        if (tempPoint > 60) tempPoint = 60;
        
        thermalData[i] = tempPoint;
    }
    
    setFullData(thermalData);
}

std::ostream& operator<<(std::ostream& os, const ThermalCamera& sensor) {
    os << "ThermalCamera #" << sensor.getSensorId() 
       << " captured thermal image (" << sensor.getThermalDescription() << ")" << endl;
    
    // Display the matrix
    const int* data = sensor.getFullData();
    const int matrixSize = static_cast<int>(sqrt(ThermalCamera::MAX_DATA_SIZE)); 
    
    os << "Thermal Matrix " << matrixSize << "x" << matrixSize 
       << " (temperatures °C):" << endl;
    
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            os << setw(3) << data[i * matrixSize + j] << " ";
        }
        os << endl;
    }
    
    return os;
}