#include <iostream>
#include "SystemManager/SystemManager.h"

/**
 * @file main.cpp
 * @brief Main entry point for the Julio Veganos e Hijos Monitoring System
 * 
 * This program provides a unified interface that combines:
 * - User management (Admin/Employee authentication and CRUD operations)
 * - Sensor monitoring (Multi-sensor data collection and coordination)
 * - Security system (Automated alarm and camera activation)
 * - Real-time dashboard (System status and environmental monitoring)
 * 
 * Features integrated from original test programs:
 * - Complete user database operations with exception handling
 * - Comprehensive sensor management with coordination testing
 * - Security alarm system with automatic camera activation
 * - Role-based access control for different user types
 * - Persistent binary database storage for both users and sensors
 * 
 * @author Carlos Nebril
 * @date June 25, 2025
 * @version 2.0
 */
int main() {
    try {
        cout << "Starting Julio Veganos e Hijos Monitoring System..." << endl;
        
        // Initialize system manager with default database files
        SystemManager system("data/users.dat", "data/sensors.dat");
        
        // Initialize all system components
        if (!system.initialize()) {
            cerr << "FATAL ERROR: System initialization failed!" << endl;
            cerr << "Please check database files and permissions." << endl;
            return 1;
        }
        
        // Run the main system loop
        system.run();
        
        cout << "\nSystem shutdown completed successfully." << endl;
        cout << "All data has been automatically saved." << endl;
        cout << "Thank you for using Julio Veganos e Hijos Monitoring System!" << endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        cerr << "CRITICAL SYSTEM ERROR: " << e.what() << endl;
        cerr << "The system will now terminate." << endl;
        return 1;
        
    } catch (...) {
        cerr << "UNKNOWN CRITICAL ERROR: An unexpected error occurred." << endl;
        cerr << "The system will now terminate." << endl;
        return 2;
    }
}