#include "./ZonePlacer.h"
#include "./Map.h"
#include "./Template.h"
#include "./Zone.h"
#include "./Tile.h"


ZonePlacer::ZonePlacer(Map & map, TemplateInfo & temp) : map(map), temp(temp) {
    mapWidth = 0;
    mapHeight = 0;
}

int3 ZonePlacer::getRealCoords(float3 f) {
    return int3(std::max(0.0f, f.x * mapWidth - 1), std::max(0.0f, f.y * mapHeight - 1), f.z);
}

void ZonePlacer::generateZones() {

    mapWidth = map.getWidth();
    mapHeight = map.getHeight();

    // auto &zones = map.getZones();
    
    std::cerr << "Generating zones\n";
    temp.printTemplate();



    auto zonesI = temp.getZonesI();
    for(auto& zoneI : zonesI) {
        map.getZones().emplace(zoneI.first, std::make_shared<Zone>(zoneI.first));
    }


    placeOnGrid();
    placeZones();

    paintTiles();
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
        std::cerr << "Distances from zone: " << start << "\n";
        for(auto& distancesFromStart : DistancesBetweenZones[start]) {
            std::cerr << "Distance to id: " << distancesFromStart.first << " is: " << distancesFromStart.second << "\n";
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

    std::cerr << "ZONE VECTOR DEBUG\n";
    for(auto zone : zoneVector) {
        std::cerr << "Zone id: " << zone.first << " " << "\n";
    }

    size_t x = 0, y = 0;

    grid[x][y] = zoneVector[0].second;

    for(size_t i = 1; i < zoneVector.size(); i++) {
        x++;
        if(x == GridN) {
            x = 0;
            y++;
        }
        grid[x][y] = zoneVector[i].second;
    }


    for (size_t x = 0; x < GridN; ++x)
	{
		for (size_t y = 0; y < GridN; ++y)
		{
			auto zone = grid[x][y];
			if (zone)
			{
                std::cerr << "X: " << x << " Y: " << y << "\n";
                std::cerr << "x / GridN: " << x / GridN << " y / GridN: " << y / GridN << "\n"; 

                float x_ = x;
                float y_ = y;

                x_ += 0.5;
                y_ += 0.5;


				zone->setCenter(float3(1.0 * x_ / GridN, 1.0 * y_ / GridN, 0));
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

            std::cerr << "Tile x: " << x << " y: " << y << " zone: " << minDistZone << "\n";
            
            auto TilePtr = map.getTile(x, y); 
            if(TilePtr) {
                std::cerr << "setting zone id\n";
                TilePtr->setZoneId(minDistZone);
                std::cerr << "Test "  << TilePtr->getZoneId() << "\n"; 
            } else {
                std::cerr << "TilePtr is null\n";
            }
            
            // map.getTile(x, y)->setZoneId(minDistZone);
        }
    }

    for(auto& zone : zones) {
        std::cerr << "Zone id: " << zone.first << "\n";
        zone.second->printZone();
    }
}