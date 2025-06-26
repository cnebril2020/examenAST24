#include "AlarmSystem.h"
#include "../Sensors/Coordination/SensorCoordinator.h"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;

AlarmSystem::AlarmSystem(SensorDatabase& sensorDb) : database(sensorDb) {
    // Simple constructor, no complex initialization needed
}

bool AlarmSystem::checkAlarm() {
    cout << "[ALARM] Checking system status..." << endl;
    
    // Query SensorCoordinator to detect movement
    bool movementDetected = SensorCoordinator::isMovementDetected();
    
    if (movementDetected) {
        cout << "[ALARM] *** MOVEMENT DETECTED - SECURITY ALERT ***" << endl;
        dumpRGBCameras();
        return true; // Suspicious activity detected
    } else {
        cout << "[ALARM] System secure - No movement detected" << endl;
        return false; // All clear
    }
}

void AlarmSystem::dumpRGBCameras() {
    vector<RGBCamera*> rgbCameras = findRGBCameras();
    
    if (rgbCameras.empty()) {
        cout << "[ALARM] No RGB cameras available for security capture" << endl;
        return;
    }
    
    // Get current timestamp
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    
    cout << "\n==================== SECURITY CAPTURE ====================" << endl;
    cout << "TIMESTAMP: " << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S") << endl;
    cout << "TRIGGER: Movement detected by Contact Sensor" << endl;
    cout << "RGB CAMERAS (" << rgbCameras.size() << " found):" << endl;
    cout << "==========================================================" << endl;
    
    for (size_t i = 0; i < rgbCameras.size(); i++) {
        RGBCamera* camera = rgbCameras[i];
        
        // Force new data capture
        camera->collectData();
        
        cout << "\n[CAMERA " << (i + 1) << "] ID: " << camera->getSensorId() << endl;
        cout << *camera << endl;
    }
    
    cout << "\n================== END SECURITY CAPTURE ==================" << endl;
}

vector<RGBCamera*> AlarmSystem::findRGBCameras() {
    vector<RGBCamera*> rgbCameras;
    vector<Sensor*> allSensors = database.getAllSensors();
    
    for (Sensor* sensor : allSensors) {
        if (sensor->getType() == Sensor::RGB_CAMERA) {
            RGBCamera* rgbCamera = dynamic_cast<RGBCamera*>(sensor);
            if (rgbCamera) {
                rgbCameras.push_back(rgbCamera);
            }
        }
    }
    
    return rgbCameras;
}