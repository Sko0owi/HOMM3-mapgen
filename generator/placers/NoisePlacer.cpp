#include "./NoisePlacer.h"
#include "../global/PerlinNoise.h"
#include "../global/Random.h"
#include "../Map.h"
#include "../gameInfo/Tile.h"
#include "../gameInfo/Zone.h"
#include "../gameInfo/Town.h"
#include "../gameInfo/Terrain.h"
#include "./ObjectPlacer.h"
#include "../types/int3.h"


NoisePlacer::NoisePlacer(Map &map, RNG *rng, std::shared_ptr<ObjectPlacer> objectPlacer) : map(map), rng(rng), objectPlacer(objectPlacer) {
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            distances[y][x] = INT_MAX;
            from[y][x] = int3(-1,-1,-1);
        }
    }
}

void NoisePlacer::placeNoise() {
    PerlinNoise perlinNoise(rng);

    auto noiseGrid = perlinNoise.generateNoiseGrid(map.getWidth(), map.getHeight(), 0.4);

    for (int y = 0; y < map.getHeight(); y++) {
        std::vector<int> row;
        for (int x = 0; x < map.getWidth(); x++) {
            row.push_back(0);
        }
        currentMap.push_back(row);
    }

    

    auto objectsMap = objectPlacer->getObjectsMap();

    for(int y = 0; y < map.getHeight(); y++) {
        for(int x = 0; x < map.getWidth(); x++) {

            auto tile = map.getTile(x, y);

            if(tile->getIsRoad() && objectsMap[y][x] <= 2) {
                currentMap[y][x] = 1;
            } else if(objectsMap[y][x] == 0) {
                currentMap[y][x] = 0;
            } else if (objectsMap[y][x] == 1) {
                currentMap[y][x] = 1;
            } else if (objectsMap[y][x] >= 2) {
                currentMap[y][x] = 2;
            }
            

        }
    }

    for(int y = 0; y < map.getHeight(); y++) {
        for(int x = 0; x < map.getWidth(); x++) {
            if(currentMap[y][x] == 0) {
                if(noiseGrid[y][x] > 0.0) {
                    currentMap[y][x] = 3;
                }
            }
        }
    }

    auto zones = map.getZones();
    for(auto& zone : zones) {
        for(auto& object : zone.second->getObjects()) {
            if(auto town = std::dynamic_pointer_cast<Town>(object)) {
                auto townPos = town->getPosition();
                
                calculateDistances(townPos);

                for(int y = 0; y < map.getHeight(); y++) {
                    for(int x = 0; x < map.getWidth(); x++) {
                        if(currentMap[y][x] == 1 && distances[y][x] != 0) {
                            fixMap(int3(x,y,0));
                        }
                    }
                }
            }
        }
    }

    for(int y = 0; y < map.getHeight(); y++) {
        for(int x = 0; x < map.getWidth(); x++) {
            if(currentMap[y][x] == 0 && distances[y][x] != 0) {
                currentMap[y][x] = 3;
            }
        }
    }



    auto mapObjects = map.getMapObjects();

    for(int y = 0; y < map.getHeight(); y++) {
        for(int x = 0; x < map.getWidth(); x++) {
            if(currentMap[y][x] == 3) {
                mapObjects.push_back(Object(int3(x, y, 0), rng->randomObstacle()));
            }
        }
    }

    map.setMapObjects(mapObjects);

}

void NoisePlacer::fixMap(int3 startPos) {
    auto [x, y, z] = startPos;

    auto nextPos = from[y][x];

    while(distances[y][x] != 0) {
        currentMap[y][x] = 0;
        distances[y][x] = 0;
        x = nextPos.x;
        y = nextPos.y;
        nextPos = from[y][x];
    }
}

void NoisePlacer::calculateDistances(int3 startPos) {

    struct Compare
    {
        bool operator()(const pair<int, int3> &a, const pair<int, int3> &b)
        {
            return a.first > b.first;
        }
    };

    priority_queue<pair<int, int3>, vector<pair<int, int3>>, Compare> pq;

    distances[startPos.y][startPos.x] = 0;
    from[startPos.y][startPos.x] = startPos;
    pq.push({0, startPos});

    int dx[8] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[8] = {-1, 1, 0, 0, -1, 1, -1, 1};

    while(!pq.empty()) {
        auto [dist, currentPos] = pq.top();
        pq.pop();

        if(dist > distances[currentPos.y][currentPos.x]) {
            continue;
        }

        auto [x, y, z] = currentPos;

        for(int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(nx < 0 || nx >= map.getWidth() || ny < 0 || ny >= map.getHeight()) {
                continue;
            }

            if(currentMap[ny][nx] == 2) {
                continue;
            }

            int newDist = dist + 1;
            if(i > 3) {
                newDist += 1;
            }

            if(currentMap[ny][nx] == 0 || currentMap[ny][nx] == 1) {
                newDist = dist;
            }


            if(newDist < distances[ny][nx]) {
                distances[ny][nx] = newDist;
                from[ny][nx] = int3(x, y, z);
                pq.push({newDist, int3(nx, ny, z)});
            }
        }
    }

}