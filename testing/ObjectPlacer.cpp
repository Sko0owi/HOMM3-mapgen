#include "./ObjectPlacer.h"

#include "./Map.h"
#include "./Template.h"
#include "./global/Random.h"
#include "./game_info/Town.h"
#include "./game_info/Zone.h"
#include "./game_info/Mine.h"
#include "./game_info/Tile.h"
#include "./game_info/Object.h"

ObjectPlacer::ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng) : map(map), temp(temp), rng(rng) {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    objectsMap.resize(mapHeight);
    for(auto& row : objectsMap) {
        row.resize(mapWidth);
    }

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            objectsMap[y][x] = 0;
        }
    }

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            auto TilePtr = map.getTile(x, y);
            if ((TilePtr->getIsBorder() || TilePtr->getIsExtension()) && !TilePtr->getIsRoad()) {
                objects.push_back(std::make_shared<Object>(int3(x, y, 0), "Border"));
                objectsMap[y][x] = 1;
            }
        }
    }
}

void ObjectPlacer::placeObjects() {
    std::cerr << "Placing objects\n";

    placeTowns();

    placeMines();

    // for(auto object : objects) {
    //     std::cerr << "Object: " << object->getName() << " at " << object->getPosition().toString() << "\n";
    // }


    for(int y = 0; y < mapHeight; y++) {
        for(int x = 0; x < mapWidth; x++) {
            std::cerr << objectsMap[y][x];
        }
        std::cerr << "\n";
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

            int x = town.getPosition().x;
            int y = town.getPosition().y;

            for(int x_ = max(0, x - town.getSizeOfObject().x + 1); x_ <= x ; x_++) {
                for(int y_ = max(0, y - town.getSizeOfObject().y + 1); y_ <= y; y_++) {

                    objectsMap[y_][x_] = 2;
                }
            }
            
            auto townPointer = std::make_shared<Town>(town);
            zonePtr->addObject(townPointer);
            
            objects.push_back(std::make_shared<Town>(town));
        }
        
    }
}

void ObjectPlacer::placeMines() {
    std::cerr << "Placing mines\n";

    auto zones = map.getZones();

    for(auto& zone : zones) {
        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        if(zonePtr->getOwnerId() == 0) continue;

        auto zoneI = temp.getZonesI()[zoneId];


        auto pos = zonePtr->getPosition();

        pos.x += 5;
        Mine mine = Mine(MineType::MINE_SAWMILL, zonePtr->getOwnerId(), pos, "Sawmill");

        mine.setSizeOfObject(int3(4,2,1));

        int x = mine.getPosition().x;
        int y = mine.getPosition().y;

        for(int x_ = max(0, x - mine.getSizeOfObject().x + 1); x_ <= x ; x_++) {
            for(int y_ = max(0, y - mine.getSizeOfObject().y + 1); y_ <= y; y_++) {

                objectsMap[y_][x_] = 3;
            }
        }

        auto minePointer = std::make_shared<Mine>(mine);

        zonePtr->addObject(minePointer);

        objects.push_back(std::make_shared<Mine>(mine));

    }
}
