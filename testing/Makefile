# Compiler settings
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Directories
SRC_DIR = .
UTILS_DIR = ../utils
GAME_INFO_DIR = ./game_info
GLOBAL_DIR = ./global
TEMPLATE_INFO_DIR = ./template_info
PLACERS_DIR = ./placers


# Include paths
INCLUDES = -I$(SRC_DIR) -I$(UTILS_DIR) -I$(GAME_INFO_DIR) -I$(GLOBAL_DIR)

# Library flags
LDFLAGS = -L/usr/lib -llua

# Source files
SOURCES = $(SRC_DIR)/test.cpp \
		  $(TEMPLATE_INFO_DIR)/ConnectionInfo.cpp \
		  $(TEMPLATE_INFO_DIR)/ZoneInfo.cpp \
		  $(TEMPLATE_INFO_DIR)/MineInfo.cpp \
		  $(TEMPLATE_INFO_DIR)/TownInfo.cpp \
		  $(TEMPLATE_INFO_DIR)/TreasuresInfo.cpp \
          $(TEMPLATE_INFO_DIR)/TemplateInfo.cpp \
          $(SRC_DIR)/Map.cpp \
		  $(GAME_INFO_DIR)/Faction.cpp \
		  $(GAME_INFO_DIR)/Terrain.cpp \
		  $(GAME_INFO_DIR)/MineType.cpp \
		  $(GAME_INFO_DIR)/Mine.cpp \
		  $(GAME_INFO_DIR)/Treasure.cpp \
		  $(GAME_INFO_DIR)/TreasureType.cpp \
		  $(GAME_INFO_DIR)/Object.cpp \
          $(GAME_INFO_DIR)/Zone.cpp  \
          $(GAME_INFO_DIR)/Tile.cpp  \
          $(GAME_INFO_DIR)/Town.cpp  \
          $(GAME_INFO_DIR)/Creature.cpp  \
          $(PLACERS_DIR)/ZonePlacer.cpp \
          $(PLACERS_DIR)/ObjectPlacer.cpp \
		  $(PLACERS_DIR)/BorderPlacer.cpp \
		  $(PLACERS_DIR)/GuardPlacer.cpp \
		  $(PLACERS_DIR)/RoadPlacer.cpp \
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