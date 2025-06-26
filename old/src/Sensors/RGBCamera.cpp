#include "RGBCamera.h"
#include "Coordination/SensorCoordinator.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>

using namespace std;

// Constructor - calls parent constructor with type
RGBCamera::RGBCamera(u_int32_t sensorId) 
    : Sensor(sensorId, Type::RGB_CAMERA) {
    collectData();
}

RGBCamera* RGBCamera::createPrimary() {
    cout << "[SYSTEM] Loading PRIMARY RGBCamera (ID=" 
         << PRIMARY_RGB_ID << ")" << endl;
    return new RGBCamera(PRIMARY_RGB_ID);
}

// Implementation of pure virtual method collectData
void RGBCamera::collectData() {
    readRGBDataFromHardware();
}

// Human-readable image quality interpretation (simplified)
const char* RGBCamera::getImageQualityDescription() const {
    // COORDINATION: Use movement state for description
    bool hasActivity = SensorCoordinator::isMovementDetected();
    
    if (hasActivity) {
        return "RGB: ACTIVITY DETECTED (Lights ON)";
    } else {
        return "RGB: NO ACTIVITY (Night Mode)";
    }
}

void RGBCamera::readRGBDataFromHardware() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
    
    int rgbData[MAX_DATA_SIZE];
    
    // COORDINATION: Use movement state instead of random scenarios
    bool hasMovement = SensorCoordinator::isMovementDetected();
    
    int baseValue;
    if (hasMovement) {
        // Movement detected -> lights ON -> bright image
        baseValue = 180;
    } else {
        // No movement -> lights OFF -> dark image
        baseValue = 80;
    }
    
    // Generate pixel values around coordinated base value
    for (size_t i = 0; i < MAX_DATA_SIZE; i++) {
        int variation = (rand() % 41) - 20; // -20 to +20
        int pixelValue = baseValue + variation;
        
        // Ensure valid range (0-255)
        if (pixelValue < 0) pixelValue = 0;
        if (pixelValue > 255) pixelValue = 255;
        
        rgbData[i] = pixelValue;
    }
    
    setFullData(rgbData);
}

std::ostream& operator<<(std::ostream& os, const RGBCamera& sensor) {
    os << "RGBCamera #" << sensor.getSensorId() 
       << " captured image (" << sensor.getImageQualityDescription() << ")" << endl;
    
    // Display the matrix
    const int* data = sensor.getFullData();
    const int matrixSize = static_cast<int>(sqrt(RGBCamera::MAX_DATA_SIZE)); 
    
    os << "RGB Matrix " << matrixSize << "x" << matrixSize 
       << " (pixel values 0-255):" << endl;
    
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            os << setw(3) << data[i * matrixSize + j] << " ";
        }
        os << endl;
    }
    
    return os;
}