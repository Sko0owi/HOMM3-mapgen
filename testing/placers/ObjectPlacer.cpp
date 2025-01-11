#include "./ObjectPlacer.h"

#include "./Map.h"
#include "../template_info/TemplateInfo.h"
#include "../template_info/ZoneInfo.h"
#include "../template_info/ConnectionInfo.h"
#include "./template_info/MineInfo.h"
#include "./template_info/TownInfo.h"
#include "./template_info/TreasuresInfo.h"
#include "./types/int3.h"
#include "./global/Random.h"
#include "./game_info/Town.h"
#include "./game_info/Zone.h"
#include "./game_info/Treasure.h"
#include "./game_info/Mine.h"
#include "./game_info/Tile.h"
#include "./game_info/Object.h"

ObjectPlacer::ObjectPlacer(Map &map, TemplateInfo &temp, RNG *rng) : map(map), temp(temp), rng(rng)
{
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    objectsMap.resize(mapHeight);
    for (auto &row : objectsMap)
    {
        row.resize(mapWidth);
    }

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            objectsMap[y][x] = 0;
        }
    }

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            auto TilePtr = map.getTile(x, y);
            if ((TilePtr->getIsBorder() || TilePtr->getIsExtension()) && !TilePtr->getIsRoad())
            {
                objectsMap[y][x] = 1;
            }
        }
    }

    std::cerr << "calculating center of zones\n";
    for (auto &zone : map.getZones())
    {
        auto zonePtr = zone.second;

        auto centerOfZone = std::make_shared<Object>(Object(zonePtr->getPosition(), "Center Of Zone", int3(5, 3, 1)));

        zonePtr->addObject(centerOfZone);
        objects[zonePtr].push_back(centerOfZone);
    }

    recalculateDistances();
}

void ObjectPlacer::placeObjects()
{
    std::cerr << "Placing objects\n";

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            auto TilePtr = map.getTile(x, y);
            auto TileZone = TilePtr->getZoneId();

            zoneTiles[map.getZones()[TileZone]].push_back({{x, y}, TilePtr});
        }
    }

    placeTowns();

    placeMines();

    placeTreasures();

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            std::cerr << objectsMap[y][x];
        }
        std::cerr << "\n";
    }
}

std::vector<Treasure> possibleTreasures[4];
void ObjectPlacer::preparePossibleTreasures()
{

    for (TreasureType j = TreasureType(TreasureType::LOW_TIER_TAB + 1); j < TreasureType(TreasureType::MID_TIER_TAB); j = TreasureType(j + 1))
    {
        possibleTreasures[0].push_back(Treasure(j, 1));
    }

    for (TreasureType j = TreasureType(TreasureType::MID_TIER_TAB + 1); j < TreasureType(TreasureType::HIGH_TIER_TAB); j = TreasureType(j + 1))
    {
        possibleTreasures[1].push_back(Treasure(j, 1));
    }

    for (TreasureType j = TreasureType(TreasureType::HIGH_TIER_TAB + 1); j < TreasureType(TreasureType::END_TIER_TAB); j = TreasureType(j + 1))
    {
        possibleTreasures[2].push_back(Treasure(j, 1));
    }

    for (TreasureType j = TreasureType(TreasureType::BUILDING_TIER_TAB + 1); j < TreasureType(TreasureType::LOW_TIER_TAB); j = TreasureType(j + 1))
    {
        possibleTreasures[3].push_back(Treasure(j, 1));
    }
    

}


void ObjectPlacer::placeBlockOfTreasures(std::shared_ptr<Zone> zonePtr, TreasuresInfo treasuresInfo)
{

    vector<int3> possiblePositions;

    int3 blockSize = possibleBlockSizes[rng->nextInt(0, possibleBlockSizes.size() - 1)];

    for (auto &tiles : zoneTiles[zonePtr])
    {
        auto [pos, tile] = tiles;
        auto [x, y] = pos;

        if (canPlaceObject(int3(x, y, 0), blockSize))
        {
            possiblePositions.push_back(int3(x, y, 0));
        }
    }

    if (possiblePositions.empty())
    {
        std::cerr << "No possible placement\n";
        return;
    }

    int rand = rng->nextInt(0, possiblePositions.size() - 1);
    auto pos = possiblePositions[rand];

    int x = pos.x;
    int y = pos.y;

    for (int x_ = max(0, x - blockSize.x); x_ <= min(x + 1, mapWidth - 1); x_++)
    {
        for (int y_ = max(0, y - blockSize.y); y_ <= min(y + 1, mapHeight - 1); y_++)
        {

            objectsMap[y_][x_] = 4;

            if (x_ == x - blockSize.x || x_ == x + 1 || y_ == y - blockSize.y || y_ == y + 1)
            {
                objectsMap[y_][x_] = 1;
            }
            else
            {

                int tierRNG = rng->nextInt(0, 100);
                int tier = 0;
                for (int i = 0; i < 3; i++)
                {
                    tierRNG -= treasuresInfo.getFrequency(ZoneRichness(i));
                    if (tierRNG <= 0)
                    {
                        tier = i;
                        break;
                    }
                }

                int rand = rng->nextInt(0, possibleTreasures[tier].size() - 1);
                Treasure treasure = possibleTreasures[tier][rand];

                treasure.setPosition(int3(x_, y_, 0));

        
                treasure.setQuantity(rng->nextInt(5,20));

                auto treasurePointer = std::make_shared<Treasure>(treasure);

                zonePtr->addObject(treasurePointer);
                objects[zonePtr].push_back(treasurePointer);
            }
        }
    }
}

void ObjectPlacer::preparePossibleBlockSizes()
{

    possibleBlockSizes.push_back(int3(4, 3, 1));
    possibleBlockSizes.push_back(int3(3, 3, 1));
    possibleBlockSizes.push_back(int3(2, 2, 1));
    possibleBlockSizes.push_back(int3(2, 3, 1));
    possibleBlockSizes.push_back(int3(3, 2, 1));
    possibleBlockSizes.push_back(int3(1, 1, 1));

}

void ObjectPlacer::placeTreasureBuilding(std::shared_ptr<Zone> zonePtr, TreasuresInfo treasuresInfo) {

    int rndTreausre = rng->nextInt(0, possibleTreasures[3].size());
    Treasure buildingToPlace = possibleTreasures[3][rndTreausre];
    
    int3 buildingSize = getTreasureSize(buildingToPlace.getTreasureType());

    vector<int3> possiblePositions;

    for (auto &tiles : zoneTiles[zonePtr])
    {
        auto [pos, tile] = tiles;
        auto [x, y] = pos;

        if (canPlaceObject(int3(x, y, 0), buildingSize))
        {
            possiblePositions.push_back(int3(x, y, 0));
        }
    }

    if (possiblePositions.empty())
    {
        std::cerr << "No possible placement\n";
        return;
    }

    int rand = rng->nextInt(0, possiblePositions.size() - 1);
    auto pos = possiblePositions[rand];

    int x = pos.x;
    int y = pos.y;

    for (int x_ = max(0, x - buildingSize.x); x_ <= min(x + 1, mapWidth - 1); x_++)
    {
        for (int y_ = max(0, y - buildingSize.y); y_ <= min(y + 1, mapHeight - 1); y_++)
        {

            objectsMap[y_][x_] = 9;

            if (x_ == x - buildingSize.x || x_ == x + 1 || y_ == y - buildingSize.y || y_ == y + 1)
            {
                objectsMap[y_][x_] = 1;
            }
        }
    }

    buildingToPlace.setPosition(int3(x,y,0));
    buildingToPlace.setSizeOfObject(buildingSize);
    buildingToPlace.setName("BUILDING TREASURE");

    auto treasurePointer = std::make_shared<Treasure>(buildingToPlace);

    zonePtr->addObject(treasurePointer);
    objects[zonePtr].push_back(treasurePointer);
}


void ObjectPlacer::placeTreasures()
{
    std::cerr << "Placing Treasures\n";

    preparePossibleTreasures();

    preparePossibleBlockSizes();


    for (int i = 0; i < 3; i++)
    {
        std::cerr << "Debug for Tier: " << i << "\n";
        for (auto &treasure : possibleTreasures[i])
        {
            std::cerr << treasureTypeToString(treasure.getTreasureType()) << " " << treasure.getQuantity() << "\n";
        }
    }


    auto zones = map.getZones();


    for (auto &zone : zones) {
        auto zonePtr = zone.second;

        for(auto& object : zonePtr->getObjects()){
            if(auto mine = std::dynamic_pointer_cast<Mine>(object)) {
                auto mineType = mine->getMineType();

                TreasureType treasureType;
                switch(mineType) {
                    case(MineType::MINE_GOLD_MINE):
                        treasureType = TreasureType::RESOURCE_GOLD;
                        break;
                    case(MineType::MINE_ORE_PIT):
                        treasureType = TreasureType::RESOURCE_ORE;
                        break;
                    case(MineType::MINE_SAWMILL):
                        treasureType = TreasureType::RESOURCE_WOOD;
                        break;
                    case(MineType::MINE_SULFUR_DUNE):
                        treasureType = TreasureType::RESOURCE_SULFUR;
                        break;
                    case(MineType::MINE_GEM_POND):
                        treasureType = TreasureType::RESOURCE_GEMS;
                        break;
                    case(MineType::MINE_CRYSTAL_CAVERN):
                        treasureType = TreasureType::RESOURCE_CRYSTAL;
                        break;
                    case(MineType::MINE_ALCHEMISTS_LAB):
                        treasureType = TreasureType::RESOURCE_MERCURY;
                        break;
                };

                for(int x_ = max(0, mine->getPosition().x - mine->getSizeOfObject().x + 2); x_ <= mine->getPosition().x - 1; x_++) {
                    
                    if(rng->nextBool(0.5)) {
                        continue;
                    }
                    
                    Treasure treasure;
                    treasure.setTreasureType(treasureType);
                    treasure.setPosition(int3(x_, mine->getPosition().y + 1, 0));

                    treasure.setQuantity(rng->nextInt(5, 20));

                    auto treasurePointer = std::make_shared<Treasure>(treasure);

                    zonePtr->addObject(treasurePointer);
                    objects[zonePtr].push_back(treasurePointer);
                                        
                }
            
            }
        }
    }


    for (auto &zone : zones)
    {

        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        TreasuresInfo treasuresInfo = temp.getZonesI()[zoneId]->getTreasuresInfo();

        ZoneRichness richness = treasuresInfo.getRichness();

        int minBlocks, maxBlocks;

        int minBuildings, maxBuildings;

        switch (richness)
        {
        case ZoneRichness::Low:
            minBlocks = 1;
            maxBlocks = 3;
            
            minBuildings = 0;
            maxBuildings = 1;
            break;
        case ZoneRichness::Normal:
            minBlocks = 2;
            maxBlocks = 4;

            minBuildings = 1;
            maxBuildings = 3;
            break;
        case ZoneRichness::Rich:
            minBlocks = 3;
            maxBlocks = 5;

            minBuildings = 2;
            maxBuildings = 5;
            break;
        }

        int numOfBlocks = rng->nextInt(minBlocks, maxBlocks);

        std::cerr << "Placing treasures in zone " << zoneId << " with " << numOfBlocks << " blocks\n";

        for (int i = 0; i < numOfBlocks; i++)
        {
            placeBlockOfTreasures(zonePtr, treasuresInfo);
        }

        int numOfBuildings = rng->nextInt(minBuildings, maxBuildings);
        for(int i = 0; i < numOfBuildings; i++) {
            placeTreasureBuilding(zonePtr, treasuresInfo);
        }
    
    }

}

void ObjectPlacer::placeTowns()
{
    std::cerr << "Placing towns\n";

    auto zones = map.getZones();

    for (auto &zone : zones)
    {
        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        auto zoneI = temp.getZonesI()[zoneId];

        for (auto &townI : zoneI->getTowns())
        {

            Town town(townI);

            town.setSizeOfObject(int3(5, 3, 1));
            town.setPosition(zonePtr->getPosition());

            int x = town.getPosition().x;
            int y = town.getPosition().y;

            for (int x_ = max(0, x - town.getSizeOfObject().x); x_ <= x + 1; x_++)
            {
                for (int y_ = max(0, y - town.getSizeOfObject().y); y_ <= y + 1; y_++)
                {

                    objectsMap[y_][x_] = 2;
                    if (x_ == x - town.getSizeOfObject().x || x_ == x + 1 || y_ == y - town.getSizeOfObject().y || y_ == y + 1)
                    {
                        objectsMap[y_][x_] = 1;
                    }
                }
            }

            auto townPointer = std::make_shared<Town>(town);
            zonePtr->addObject(townPointer);

            objects[zonePtr].push_back(townPointer);
        }
    }

    recalculateDistances();
}

bool ObjectPlacer::canPlaceObject(int3 pos, int3 size)
{   
    

    int x = pos.x;
    int y = pos.y;

    if (y == mapHeight - 1 || y == mapHeight - 2)
        return false;

    if (x - size.x + 1 < 0 || y - size.y + 1 < 0)
        return false;

    for (int x_ = max(0, x - size.x + 1); x_ <= x; x_++)
    {
        for (int y_ = max(0, y - size.y + 1); y_ <= y; y_++)
        {
            if (objectsMap[y_][x_] >= 1)
                return false;

            auto TilePtr = map.getTile(x_, y_);
            if (TilePtr->getIsRoad())
                return false;
            if (TilePtr->getZoneId() != map.getTile(x, y)->getZoneId())
                return false;
        }
    }

    return true;
}

bool ObjectPlacer::placeMine(MineInfo mineI, std::shared_ptr<Object> centerPtr, std::shared_ptr<Zone> zonePtr, bool firstBasicMine, bool deterministic)
{

    int3 mineSize = getMineSize(mineI.getMineType());

    vector<int3> possiblePositions;

    if (!deterministic)
    {

        for (auto &tiles : zoneTiles[zonePtr])
        {

            auto [pos, tile] = tiles;
            auto [x, y] = pos;

            if ((!firstBasicMine || objectsDistances[centerPtr][y][x] < 15) && canPlaceObject(int3(x, y, 0), mineSize))
            {
                possiblePositions.push_back(int3(x, y, 0));
            }
        }
    }
    else
    {

        int maxMin = 0;
        auto bestPos = int3(-1, -1, 0);
        for (auto &tiles : zoneTiles[zonePtr])
        {
            auto [pos, tile] = tiles;
            auto [x, y] = pos;

            if (!canPlaceObject(int3(x, y, 0), mineSize))
                continue;

            int localMin = INT_MAX;
            for (auto &object : objects[zonePtr])
            {
                if (object->getName() != "Mine")
                    continue;
                localMin = min(localMin, objectsDistances[object][y][x]);
            }
            if (localMin > maxMin)
            {
                maxMin = localMin;
                bestPos = int3(x, y, 0);
            }
        }
        if(bestPos.x != -1)
            possiblePositions.push_back(bestPos);
    }

    if (possiblePositions.empty())
    {
        std::cerr << "No possible placement\n";
        return false;
    }

    int rand = rng->nextInt(0, possiblePositions.size() - 1);
    auto pos = possiblePositions[rand];

    Mine mine(mineI);

    mine.setPosition(pos);
    mine.setSizeOfObject(mineSize);

    int x = mine.getPosition().x;
    int y = mine.getPosition().y;

    for (int x_ = max(0, x - mine.getSizeOfObject().x); x_ <= min(x + 1, mapWidth - 1); x_++)
    {
        for (int y_ = max(0, y - mine.getSizeOfObject().y); y_ <= min(y + 1, mapHeight - 1); y_++)
        {

            objectsMap[y_][x_] = 3;
            if (x_ == x - mine.getSizeOfObject().x || x_ == x + 1 || y_ == y - mine.getSizeOfObject().y || y_ == y + 1)
            {
                objectsMap[y_][x_] = 1;
            }
        }
    }

    auto minePointer = std::make_shared<Mine>(mine);

    zonePtr->addObject(minePointer);
    objects[zonePtr].push_back(minePointer);

    return true;
}

void ObjectPlacer::placeMines()
{
    std::cerr << "Placing mines\n";

    auto zones = map.getZones();

    for (auto &zone : zones)
    {
        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        auto zoneI = temp.getZonesI()[zoneId];

        std::cerr << "Placing mines in zone " << zoneId << "\n";

        std::shared_ptr<Object> centerPtr = nullptr;
        for (auto &object : zone.second->getObjects())
        {
            if (object->getName() == "Center Of Zone")
            {
                centerPtr = object;
                break;
            }
        }

        if (centerPtr == nullptr)
        {
            std::cerr << "No center of zone\n";
            assert(false);
        }

        for (auto &mineI : zoneI->getMines())
        {
            if (mineI.getMineType() == MineType::MINE_SAWMILL || mineI.getMineType() == MineType::MINE_ORE_PIT)
            {
                placeMine(mineI, centerPtr, zonePtr, true);
                zoneI->setMaxMinesCount(zoneI->getMaxMinesCount() - 1);
                mineI.setMinCount(mineI.getMinCount() - 1);
            }
        }

        for (auto &mineI : zoneI->getMines())
        {
            for (int i = 0; i < mineI.getMinCount(); i++)
            {
                placeMine(mineI, centerPtr, zonePtr);
                zoneI->setMaxMinesCount(zoneI->getMaxMinesCount() - 1);
            }
        }

        std::vector<MineInfo> minesToPlace;
        for (auto &mineI : zoneI->getMines())
        {
            minesToPlace.push_back(mineI);
        }

        int minesToPlaceCount = zoneI->getMaxMinesCount();
        while (minesToPlaceCount > 0)
        {

            recalculateDistances();

            int rand = rng->nextInt(0, minesToPlace.size() - 1);
            if (placeMine(minesToPlace[rand], centerPtr, zonePtr, false, true) == false)
            {
                std::cerr << "No possible placement\n";
                break;
            }
            minesToPlaceCount--;
        }
    }

    recalculateDistances();
}

void ObjectPlacer::recalculateDistances()
{

    std::cerr << "STARTING RECALC\n";
    for (auto &zone : map.getZones())
    {
        auto zonePtr = zone.second;
        for (auto &object : objects[zonePtr])
        {
            objectsDistances[object].clear();

            objectsDistances[object].resize(mapHeight);
            for (auto &row : objectsDistances[object])
            {
                row.resize(mapWidth);
            }

            calculateShortestDistances(object);
        }
    }
}

void ObjectPlacer::calculateShortestDistances(std::shared_ptr<Object> &object)
{

    auto pos = object->getPosition();
    struct Compare
    {
        bool operator()(const pair<int, int3> &a, const pair<int, int3> &b)
        {
            return a.first > b.first;
        }
    };

    priority_queue<pair<int, int3>, vector<pair<int, int3>>, Compare> pq;

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            objectsDistances[object][y][x] = INT_MAX;
        }
    }

    int dx[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[8] = {-1, 1, 0, 0, -1, 1, -1, 1};

    pq.push({0, pos});

    objectsDistances[object][pos.y][pos.x] = 0;

    while (!pq.empty())
    {

        auto [dist, currentPos] = pq.top();
        pq.pop();

        if (dist > objectsDistances[object][currentPos.y][currentPos.x])
            continue;

        auto [x, y, z] = currentPos;

        for (int i = 0; i < 8; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight)
                continue;

            if (objectsMap[ny][nx] >= 2)
                continue;

            int newDist = dist + 2;
            if (i > 3)
                newDist += 1;

            if (newDist < objectsDistances[object][ny][nx])
            {
                objectsDistances[object][ny][nx] = newDist;
                pq.push({newDist, int3(nx, ny, z)});
            }
        }
    }
}

std::vector<std::vector<int>> ObjectPlacer::getObjectsMap(){
    return objectsMap;
}