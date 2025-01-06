# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Directories
SRC_DIR = .
UTILS_DIR = ../utils
GAME_INFO_DIR = ./game_info
GLOBAL_DIR = ./global


# Include paths
INCLUDES = -I$(SRC_DIR) -I$(UTILS_DIR) -I$(GAME_INFO_DIR) -I$(GLOBAL_DIR)

# Library flags
LDFLAGS = -L/usr/lib -llua

# Source files
SOURCES = $(SRC_DIR)/test.cpp \
          $(SRC_DIR)/Template.cpp \
          $(SRC_DIR)/Map.cpp \
		  $(GAME_INFO_DIR)/Faction.cpp \
		  $(GAME_INFO_DIR)/Terrain.cpp \
		  $(GAME_INFO_DIR)/Object.cpp \
          $(GAME_INFO_DIR)/Zone.cpp  \
          $(GAME_INFO_DIR)/Tile.cpp  \
          $(GAME_INFO_DIR)/Town.cpp  \
          $(SRC_DIR)/ZonePlacer.cpp \
          $(SRC_DIR)/ObjectPlacer.cpp \
		  $(SRC_DIR)/BorderPlacer.cpp \
		  $(SRC_DIR)/RoadPlacer.cpp \
          $(SRC_DIR)/ObjectPlacer.cpp \
		  $(GLOBAL_DIR)/Random.cpp \
		  $(GLOBAL_DIR)/PenroseTiling.cpp \
          $(UTILS_DIR)/lua_helpers.cpp 

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Create necessary directories for object files
OBJECT_DIRS = $(sort $(dir $(OBJECTS)))

# Main target
TARGET = test

# Default target
all: $(TARGET)

# Create directories
$(OBJECT_DIRS):
	mkdir -p $@

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compilation (depend on directories)
%.o: %.cpp | $(OBJECT_DIRS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TARGET)

# Build and run
run: all
	./$(TARGET)

# Phony targets
.PHONY: all clean run