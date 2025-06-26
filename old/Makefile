# Makefile for Julio Veganos e Hijos Monitoring System
# Professional project structure

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
DATA_DIR = data

# Executable name
TARGET = monitoring_system

# Source files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/SystemManager/SystemManager.cpp \
          $(SRC_DIR)/Users/User.cpp \
          $(SRC_DIR)/Users/Admin.cpp \
          $(SRC_DIR)/Users/Employee.cpp \
          $(SRC_DIR)/Users/UserFactory.cpp \
          $(SRC_DIR)/Sensors/Sensor.cpp \
          $(SRC_DIR)/Sensors/Hygrometer.cpp \
          $(SRC_DIR)/Sensors/AirQualitySensor.cpp \
          $(SRC_DIR)/Sensors/LuxMeterSensor.cpp \
          $(SRC_DIR)/Sensors/TemperatureSensor.cpp \
          $(SRC_DIR)/Sensors/ContactSensor.cpp \
          $(SRC_DIR)/Sensors/ThermalCamera.cpp \
          $(SRC_DIR)/Sensors/RGBCamera.cpp \
          $(SRC_DIR)/Sensors/SensorFactory.cpp \
          $(SRC_DIR)/Sensors/Coordination/SensorCoordinator.cpp \
          $(SRC_DIR)/AlarmSystem/AlarmSystem.cpp \
          $(SRC_DIR)/Databases/Database.cpp \
          $(SRC_DIR)/Databases/UserDatabase.cpp \
          $(SRC_DIR)/Databases/SensorDatabase.cpp \
          $(SRC_DIR)/Databases/Exceptions/UserDatabaseException.cpp \
          $(SRC_DIR)/Utils/InputUtils.cpp

# Object files (in build directory)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: directories $(BIN_DIR)/$(TARGET)

# Create directories
directories:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(DATA_DIR)
	@mkdir -p $(BUILD_DIR)/SystemManager $(BUILD_DIR)/Users $(BUILD_DIR)/Sensors
	@mkdir -p $(BUILD_DIR)/Sensors/Coordination $(BUILD_DIR)/AlarmSystem
	@mkdir -p $(BUILD_DIR)/Databases $(BUILD_DIR)/Databases/Exceptions $(BUILD_DIR)/Utils

# Build the executable
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
	@echo "Build completed successfully!"
	@echo "Run with: ./$(BIN_DIR)/$(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN_DIR)/$(TARGET)
	@echo "Clean completed!"

# Clean everything
distclean: clean
	rm -f $(DATA_DIR)/*.dat
	rm -f *.dat
	@echo "Deep clean completed!"

# Run the program
run: all
	./$(BIN_DIR)/$(TARGET)


# Help
help:
	@echo "Available commands:"
	@echo "  make        - Build the project"
	@echo "  make clean  - Remove build files"
	@echo "  make distclean - Remove build files and data files (deep clean)"
	@echo "  make run    - Build and run the program"
	@echo "  make help   - Show this help"

.PHONY: all clean distclean run install help directories