#include "./ZonePlacer.h"
#include "./Map.h"
#include "./Template.h"
#include "./Zone.h"
#include "./Tile.h"
#include "./global/Random.h"
#include "./game_info/Town.h"

ZonePlacer::ZonePlacer(Map & map, TemplateInfo & temp, RNG *rng) : map(map), temp(temp), rng(rng) {
    mapWidth = 0;
    mapHeight = 0;
}

int3 ZonePlacer::getRealCoords(float3 f) {
    return int3(std::max(0.0f, f.x * mapWidth - 1), std::max(0.0f, f.y * mapHeight - 1), f.z);
}

void ZonePlacer::generateZones() {

    std::cerr << "Generating zones\n";

    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    auto zonesI = temp.getZonesI();
    for(auto& zoneI : zonesI) {
        map.getZones().emplace(zoneI.first, std::make_shared<Zone>(zoneI.first));

        map.getZones()[zoneI.first]->setTerrain(zoneI.second->getTerrain());

        for (auto& town : zoneI.second->getTowns()) {
            map.getZones()[zoneI.first]->addTown(town);
        }

        map.getZones()[zoneI.first]->setOwnerId(zoneI.second->getOwner());
    }


    placeOnGrid();
    placeZones();

    paintTiles();
    determineZoneEdges();
    modifyRandomConnectionTile();
}

void ZonePlacer::calculatePaths() {

    std::cerr << "Calculating paths\n";
    auto zonesI = temp.getZonesI();

    for (auto& zoneI : zonesI) {

        int start = zoneI.first;
        DistancesBetweenZones[start][start] = 0;
        std::queue<int> q;
        std::map<int, bool> visited;
        visited[start] = true;
        q.push(start);
        while(!q.empty()) {
            int current = q.front();
            q.pop();
            const auto& currentZoneI = zonesI[current];
            const auto& currentZoneIConnections = currentZoneI->getConnections();

            for (auto connection : currentZoneIConnections) {
                int next = connection.getOtherZone(current);
                if(next == current) continue;
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                    DistancesBetweenZones[start][next] = DistancesBetweenZones[start][current] + 1;
                }
            }

        }

        if (debug)
        {
            std::cerr << "Distances from zone: " << start << "\n";
            for(auto& distancesFromStart : DistancesBetweenZones[start]) {
                std::cerr << "Distance to id: " << distancesFromStart.first << " is: " << distancesFromStart.second << "\n";
            }
        }
    }

}

void ZonePlacer::placeOnGrid() {

    std::cerr << "Placing zones on grid\n";
    calculatePaths();


    size_t GridN = std::ceil(std::sqrt(temp.getZonesI().size()));
    std::cerr << "GridN: " << GridN << "\n";

    std::shared_ptr<Zone> grid[GridN][GridN];

    auto zones = map.getZones();

    std::vector<std::pair<int, std::shared_ptr<Zone>>> zoneVector(zones.begin(), zones.end());

    size_t x = 0, y = 0;

    grid[x][y] = zoneVector[0].second;

    for(size_t i = 1; i < zoneVector.size(); i++) {
        
        auto zone = zoneVector[i].second;

        float maxDist = -1000;
        pair<int,int> maxDistPos = {-1,-1};

        for(size_t freeX = 0; freeX < GridN; freeX++) {
            for(size_t freeY = 0; freeY < GridN; freeY++) {
                if(!grid[freeX][freeY]) {

                    
                    float dist = 0;

                    pair<int,int> potentialPos = {freeX, freeY};
                    
                    for(size_t takenX = 0; takenX < GridN; takenX++) {
                        for(size_t takenY = 0; takenY < GridN; takenY++) {
                            auto takenZone = grid[takenX][takenY];
                            if(takenZone) {

                                float localDist = DistancesBetweenZones[takenZone->getId()][zone->getId()];
                                if(localDist != 1) {
                                    localDist = std::sqrt((freeX - takenX) * (freeX - takenX) + (freeY - takenY) * (freeY - takenY)) * (localDist + 1);
                                } else {
                                    localDist = -std::sqrt((freeX - takenX) * (freeX - takenX) + (freeY - takenY) * (freeY - takenY));
                                }
                                dist += localDist;
                            }
                            
                        }
                    }

                    if(dist > maxDist) {
                        maxDist = dist;
                        maxDistPos = potentialPos;
                    }
                }
            }
        }

        grid[maxDistPos.first][maxDistPos.second] = zoneVector[i].second;
    }


    for (size_t x = 0; x < GridN; ++x)
	{
		for (size_t y = 0; y < GridN; ++y)
		{
			auto zone = grid[x][y];
			if (zone)
			{
                float x_ = x;
                float y_ = y;

                x_ = rng->nextFloat(x_ + 0.25f, x_ + 0.75f);
                y_ = rng->nextFloat(y_ + 0.25f, y_ + 0.75f);

                x_ = min(x_, (float)(GridN) - 0.5f);
                x_ = max(x_, 0.5f);

                y_ = min(y_, (float)(GridN) - 0.5f);
                y_ = max(y_, 0.5f);



				zone->setCenter(float3(x_ / GridN, y_ / GridN, 0));
			}
		}
	}

}

void ZonePlacer::placeZones() {
    std::cerr << "Placing zones on real Map\n";

    auto zones = map.getZones();
    for(auto& zone : zones) {
        auto realCoords = getRealCoords(zone.second->getCenter());
        zone.second->setPosition(realCoords);
    }
}

void ZonePlacer::paintTiles() {
    std::cerr << "Painting tiles\n";
    auto zones = map.getZones();

    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {
            int minDist = 1000000;
            int minDistZone = -1;
            for(auto& zone : zones) {
                int dist = std::abs(zone.second->getPosition().x - x) + std::abs(zone.second->getPosition().y - y);
                if(dist < minDist) {
                    minDist = dist;
                    minDistZone = zone.first;
                }
            }

            
            
            auto TilePtr = map.getTile(x, y); 
            if(TilePtr) {
                TilePtr->setZoneId(minDistZone);
            } else {
                std::cerr << "TilePtr is null\n";
            }
        }
    }
}

bool ZonePlacer::areConnected(int ZoneA, int ZoneB){
    auto zonesI = temp.getZonesI();

    for(auto e : zonesI){
        for(auto c : e.second->getConnections()){
            if ((c.getZoneA() == ZoneA && c.getZoneB() == ZoneB) ||
                (c.getZoneA() == ZoneB && c.getZoneB() == ZoneA)) {
                return true;
            }
        }
    }
    return false;
}

std::pair<int, int> ZonePlacer::getRandomConnectionPoint(int zoneA, int zoneB) {
    auto key = std::make_pair(std::min(zoneA, zoneB), std::max(zoneA, zoneB));
    auto& points = connections[key];
    if (points.empty()) return {-1, -1};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, points.size() - 1);

    return points[dist(gen)];
}

void ZonePlacer::addConnectionPoint(int zoneA, int zoneB, int x, int y) {
    auto key = std::make_pair(std::min(zoneA, zoneB), std::max(zoneA, zoneB));
    connections[key].emplace_back(x, y);
}

void ZonePlacer::determineZoneEdges() {
    std::cerr << "Determining zone edges\n";

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            auto TilePtr = map.getTile(x, y);

            int currentZoneId = TilePtr->getZoneId();
            bool isEdge = false;

            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                auto NeighborTilePtr = map.getTile(nx, ny);

                if (NeighborTilePtr && NeighborTilePtr->getZoneId() != currentZoneId) {
                    isEdge = true;
                    addConnectionPoint(currentZoneId, NeighborTilePtr->getZoneId(), x, y);
                }
            }

            TilePtr->setIsEdge(isEdge);

            if (debug) {
                std::cerr << "Edge Tile: (" << x << ", " << y << ")\n";
            }
        }
    }
}

void ZonePlacer::modifyRandomConnectionTile(int range) {
    std::cerr << "Finding connection points\n";
    auto zonesI = temp.getZonesI();
    std::set<std::pair<int, int>> processedConnections;
    int zone1, zone2;

    for(auto e : zonesI){
        for(auto c : e.second->getConnections()){
            zone1 = c.getZoneA();
            zone2 = c.getZoneB();
            
            auto connectionPair = std::make_pair(std::min(zone1, zone2), std::max(zone1, zone2));

            if (processedConnections.find(connectionPair) != processedConnections.end()) {
                continue;
            }

            processedConnections.insert(connectionPair);

            auto randomPoint = getRandomConnectionPoint(zone1, zone2);
            if (randomPoint.first == -1 && randomPoint.second == -1) continue; // No connection point found

            int x = randomPoint.first;
            int y = randomPoint.second;

            for (int dy = -range; dy <= range; ++dy) {
                for (int dx = -range; dx <= range; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    auto TilePtr = map.getTile(nx, ny);
                    if (TilePtr) {
                        TilePtr->setIsEdge(false);
                    }
                }
            }
            
            if(debug){
                std::cerr << "Modified tiles around connection point (" << x << ", " << y << ") for zones " 
                        << zone1 << " -> " << zone2 << "\n";
            }
        }
    }
}
