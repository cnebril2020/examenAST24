# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Directories - adjusted for running from userSetTest directory
SRC_DIR = ../../..
USER_DIR = $(SRC_DIR)/src/Users
DB_DIR = $(SRC_DIR)/src/Databases
TEST_DIR = $(DB_DIR)/userSetTest
EXCEPTIONS_DIR = $(DB_DIR)/Exceptions
UTILS_DIR = $(SRC_DIR)/src/Utils
BIN_DIR = bin
OBJ_DIR = obj

# Source files
USER_SRCS = $(USER_DIR)/User.cpp $(USER_DIR)/Admin.cpp $(USER_DIR)/Employee.cpp $(USER_DIR)/UserFactory.cpp
DB_SRCS = $(DB_DIR)/Database.cpp $(DB_DIR)/UserDatabase.cpp
EXCEPTIONS_SRCS = $(EXCEPTIONS_DIR)/UserDatabaseException.cpp
UTILS_SRCS = $(UTILS_DIR)/InputUtils.cpp
MAIN_SRC = $(TEST_DIR)/main.cpp

# All source files
SRCS = $(USER_SRCS) $(DB_SRCS) $(EXCEPTIONS_SRCS) $(UTILS_SRCS) $(MAIN_SRC)

# Object files - now stored in obj directory with path structure flattened
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

# Create a list of source directories for vpath
SRC_DIRS = $(sort $(dir $(SRCS)))

# Use vpath to help make find the source files
vpath %.cpp $(SRC_DIRS)

# Executable name
TARGET_NAME = userdb_test
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