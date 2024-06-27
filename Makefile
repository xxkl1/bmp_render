ARCH = $(shell uname -m)

ifeq ($(ARCH), x86_64)
    SFML_DIR = third_party/sfml_2.6.1_x86_64
else ifeq ($(ARCH), arm64)
    SFML_DIR = third_party/sfml_2.6.1_arm64
else
    $(error Unsupported architecture: $(ARCH))
endif

# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++17 -Iinclude -I${SFML_DIR}/include -Isrc

# Linker flags
LDFLAGS = -L${SFML_DIR}/lib -lsfml-graphics -lsfml-window -lsfml-system -rpath @executable_path/../${SFML_DIR}/lib

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure the bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean target
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

.PHONY: all clean
