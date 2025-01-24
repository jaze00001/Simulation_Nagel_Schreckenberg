# Compiler
CXX = g++

# Compiler-Options
CXXFLAGS = -std=c++17 -Wall -Werror -Iinclude

# File-Name
TARGET = simulation

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# source and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Default-Target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Object-Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directory if it does not exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up object files and executable
clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(TARGET).exe del $(TARGET).exe