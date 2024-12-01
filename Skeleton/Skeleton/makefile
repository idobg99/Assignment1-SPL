# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# Compiler and Flags
CXX = g++
CXXFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Source and Object Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)  
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SOURCES))

# Target Executable
TARGET = $(BIN_DIR)/simulation

# Default Rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp 
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

# Phony Targets
.PHONY: all clean
