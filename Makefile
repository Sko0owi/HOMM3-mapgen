CC ?= gcc
CXX = g++
CXXFLAGS = -Wall -std=c++17

INC := -I homm3tools/h3m/h3mlib -I homm3tools/3rdparty/uthash/src
LIBDIR := homm3tools/OUTPUT/gcc
STATICLIBS := $(LIBDIR)/h3mlib.a $(LIBDIR)/h3mtilespritegen.a

LUAC ?= $(shell pkg-config --cflags lua)
LUAL ?= $(shell pkg-config --libs   lua)

CFLAGS := $(LUAC) -W -Wall -Wextra -O3 -fPIC -shared -std=c99 $(INC)
LDLIBS := $(LUAL)
LFLAGS := -L $(LIBDIR)

# Library flags
LDFLAGS = -L/usr/lib -llua

HOMM3_SRC := $(shell find homm3lua -type f -name '*.c')
HOMM3_OBJ := $(addprefix dist/,$(notdir $(HOMM3_SRC:.c=.o)))

# Directories
GENERATOR_DIR = ./generator
SRC_DIR = $(GENERATOR_DIR)/.
UTILS_DIR = $(GENERATOR_DIR)/luaUtils
GAME_INFO_DIR = $(GENERATOR_DIR)/gameInfo
GLOBAL_DIR = $(GENERATOR_DIR)/global
TEMPLATE_INFO_DIR = $(GENERATOR_DIR)/templateInfo
PLACERS_DIR = $(GENERATOR_DIR)/placers

# Source files for Generator
GEN_SOURCES = $(SRC_DIR)/Generator.cpp \
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
              $(PLACERS_DIR)/NoisePlacer.cpp \
              $(GLOBAL_DIR)/Random.cpp \
              $(GLOBAL_DIR)/PerlinNoise.cpp \
              $(GLOBAL_DIR)/PenroseTiling.cpp \
              $(UTILS_DIR)/lua_helpers.cpp 


# Object files for Generator
GEN_OBJECTS = $(GEN_SOURCES:.cpp=.o)

# Create necessary directories for object files
OBJECT_DIRS = $(sort $(dir $(GEN_OBJECTS))) dist

# Main targets
all: dist/homm3lua.so Generator

# Create directories
$(OBJECT_DIRS):
	mkdir -p $@

# Generator target
Generator: $(GEN_OBJECTS)
	$(CXX) $(GEN_OBJECTS) -o $@ $(LDFLAGS)

# HOMM3 libraries
.PHONY: libs
libs:
	$(MAKE) -C homm3tools/h3m/h3mtilespritegen/BUILD/gcc install
	$(MAKE) -C homm3tools/h3m/h3mlib/BUILD/gcc install

# HOMM3 Lua target
dist/homm3lua.so: $(HOMM3_OBJ) | libs
	$(CC) $^ $(STATICLIBS) $(CFLAGS) $(LDLIBS) -o $@

# Compilation rules
%.o: %.cpp | $(OBJECT_DIRS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

dist/%.o: homm3lua/%.c homm3lua/%.h | dist
	$(CC) $< -c $(CFLAGS) -o $@

# Documentation
doc: doc/index.html
doc/index.html: $(HOMM3_SRC)
	ldoc -c /dev/null homm3lua

# Clean
.PHONY: clean
clean:
	$(MAKE) -C homm3tools/h3m/h3mtilespritegen/BUILD/gcc clean
	$(MAKE) -C homm3tools/h3m/h3mlib/BUILD/gcc clean
	rm -rf dist doc $(GEN_OBJECTS) Generator

# Build and run Generator
.PHONY: run
run: Generator
	./Generator

# Phony targets
.PHONY: all doc clean run