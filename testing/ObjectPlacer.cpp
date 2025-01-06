#include "./ObjectPlacer.h"

#include "./Map.h"
#include "./Template.h"
#include "./global/Random.h"
#include "./game_info/Town.h"
#include "./game_info/Zone.h"
#include "./game_info/Tile.h"
#include "./game_info/Object.h"

ObjectPlacer::ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng) : map(map), temp(temp), rng(rng) {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            auto TilePtr = map.getTile(x, y);
            if(TilePtr->getIsBorder()){
                objects.push_back(std::make_shared<Object>(int3(x, y, 0), "Border"));
            }
        }
        std::cerr << "\n";
    }
}

void ObjectPlacer::placeObjects() {
    std::cerr << "Placing objects\n";

    placeTowns();

    for(auto object : objects) {
        std::cerr << "Object: " << object->getName() << " at " << object->getPosition().toString() << "\n";
    }
}

void ObjectPlacer::placeTowns() {
    std::cerr << "Placing towns\n";

    auto zones = map.getZones();

    for(auto& zone : zones) {
        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        auto zoneI = temp.getZonesI()[zoneId];

        for (auto& town : zoneI->getTowns()) {
            
            town.setSizeOfObject(int3(5,3,1));
            town.setPosition(zonePtr->getPosition());

            map.addTown(town);
            
            objects.push_back(std::make_shared<Town>(town));
        }
        
    }
}
