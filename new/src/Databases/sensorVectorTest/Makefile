# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Directories - adjusted for running from sensorVectorTest directory
SRC_DIR = ../../..
SENSOR_DIR = $(SRC_DIR)/src/Sensors
COORDINATION_DIR = $(SENSOR_DIR)/Coordination
DB_DIR = $(SRC_DIR)/src/Databases
ALARM_DIR = $(SRC_DIR)/src/AlarmSystem
TEST_DIR = $(DB_DIR)/sensorVectorTest
UTILS_DIR = $(SRC_DIR)/src/Utils
BIN_DIR = bin
OBJ_DIR = obj

# Source files
SENSOR_SRCS = $(SENSOR_DIR)/Sensor.cpp $(SENSOR_DIR)/TemperatureSensor.cpp $(SENSOR_DIR)/Hygrometer.cpp $(SENSOR_DIR)/AirQualitySensor.cpp $(SENSOR_DIR)/LuxMeterSensor.cpp $(SENSOR_DIR)/RGBCamera.cpp $(SENSOR_DIR)/ThermalCamera.cpp $(SENSOR_DIR)/ContactSensor.cpp $(SENSOR_DIR)/SensorFactory.cpp
COORDINATION_SRCS = $(COORDINATION_DIR)/SensorCoordinator.cpp
DB_SRCS = $(DB_DIR)/Database.cpp $(DB_DIR)/SensorDatabase.cpp
ALARM_SRCS = $(ALARM_DIR)/AlarmSystem.cpp
UTILS_SRCS = $(UTILS_DIR)/InputUtils.cpp
MAIN_SRC = $(TEST_DIR)/main.cpp

# All source files
SRCS = $(SENSOR_SRCS) $(COORDINATION_SRCS) $(DB_SRCS) $(ALARM_SRCS) $(UTILS_SRCS) $(MAIN_SRC)

# Object files - now stored in obj directory with path structure flattened
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

# Create a list of source directories for vpath
SRC_DIRS = $(sort $(dir $(SRCS)))

# Use vpath to help make find the source files
vpath %.cpp $(SRC_DIRS)

# Executable name
TARGET_NAME = sensordb_test
TARGET = $(BIN_DIR)/$(TARGET_NAME)

# Default target
all: directories $(TARGET)

# Create directories
.PHONY: directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Generic compilation rule for all source files
$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Run target
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Debug target
.PHONY: debug
debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all      - Build the sensor database test program (default)"
	@echo "  clean    - Remove all build files"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug flags"
	@echo "  help     - Show this help message"

# Show variables (for debugging makefile)
.PHONY: vars
vars:
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "SENSOR_SRCS: $(SENSOR_SRCS)"
	@echo "COORDINATION_SRCS: $(COORDINATION_SRCS)"
	@echo "DB_SRCS: $(DB_SRCS)"
	@echo "ALARM_SRCS: $(ALARM_SRCS)"
	@echo "TARGET: $(TARGET)"
	@echo "OBJS: $(OBJS)"