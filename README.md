# HOMM3-mapgen
Template-Based Map Generator for Heroes of Might &amp; Magic III


# Using 
* https://github.com/radekmie/homm3lua/tree/master
* https://github.com/vcmi/vcmi/tree/develop

# Pre requirements 
* Lua5.4, qt5-tools, boost

On Ubuntu you can run
* sudo apt-get install lua5.4 liblua5.4-dev libtbb-dev libsdl2-ttf-dev qttools5-dev libsdl2-mixer-dev libsdl2-image-dev



# Current demo testing
0. Ensure that tou have all of requirements installed

1. Ensure that all submodules are updated
```
git submodule update --init --recursive

```
2. Build local VCMI
```
mkdir build
cd build
cmake -S ../vcmi

cmake --build . -j8

cd ..
```

Setup vcmi via /bin/vcmilauncher (add game files)

3. make & run generator
```
make all
./Generator --seed <seed for RNG> --location <save location for map>
```



And to check results
```
/bin/vcmieditor
```

File Structure
- gameInfo/
    - Faction.h - enum of possible factions in HoMM3 + functionality
    - MineType.h - enum of possible mine types in HoMM3 + functionality
    - Terrain.h - enum of possible terrains + functionality
    - Object.h - Object class with basic object information (position, size)
    - Creature.h - Creature class (all information about guards)
    - Mine.h - Mine class (all information about mines), inherits from Object
    - Town.h - Town class (all information about towns), inherits from Object
    - Treasure.h - Treasure class (all information about treasures), inherits from Object
    - Tile.h - Tile class (abstraction for pixel), which zone it belongs to, whether it has a border, road, etc.
    - Zone.h - Zone class (abstraction for Zones), Zone center, what terrain it should have, what objects are in this zone
- global/
    - Global.h - some basic using statements (possibly to be removed)
    - Random.h - RNG class written Python-style, to easily generate random numbers from a given range
    - PenroseTiling.h - PenroseTiling class used to create Penrose Tiling
    - PerlinNoise.h - PerlinNoise class used to generate Perlin Noise
- luaUtils/
    - lua_helpers.h - Helper functions for Lua script
- types/
    - float3.h - float3 class
    - int3.h - int3 class
- templateInfo/
    - ConnectionInfo.h - Information about connections
    - MineInfo.h - Information about mines
    - TemplateInfo.h - Information about Template
    - TownInfo.h - Information about towns
    - ZoneInfo.h - Information about Zone
    - TreasureInfo.h - Information about treasures
- placers/
    - BorderPlacer.h - BorderPlacer class responsible for placing Borders
    - ObjectPlacer.h - ObjectPlacer class responsible for placing objects
    - GuardPlacer.h - GuardPlacer class responsible for placing guards
    - NoisePlacer.h - NoisePlacer class responsible for placing noise
    - RoadPlacer.h - RoadPlacer class responsible for placing roads
    - ZonePlacer.h - ZonePlacer class responsible for placing Zones
- ./
    - Map.h - Map class (abstraction for Map, all information stored here should reflect the actual map)
    - Generator.cpp - Main program responsible for creating the map (creates a Lua script which it then runs)
