#include "./ZonePlacer.h"
#include "./Map.h"
#include "./Template.h"
#include "./Zone.h"
#include "./Tile.h"
#include "./global/Random.h"
#include "./global/PenroseTiling.h"
#include "./game_info/Town.h"


ZonePlacer::ZonePlacer(Map & map, TemplateInfo & temp, RNG *rng) : map(map), temp(temp), rng(rng) {
    mapWidth = 0;
    mapHeight = 0;
}

int3 ZonePlacer::getRealCoords(float3 f) {
    return int3(std::max(0.0f, (f.x * mapWidth) - 2), std::max(0.0f, f.y * mapHeight - 1), f.z);
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
    std::cerr << zonesI.size() << "\n";

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

    prepareZones();

    TBestSolution bestSolution;

	TForceVector forces;
	TForceVector totalForces;
	TDistanceVector distances;
	TDistanceVector overlaps;

    // bool check = evaluateZones(distances, overlaps, bestSolution);
    // std::cerr << "Check: " << check << "\n";


    for (stifness = stiffnessConstant / zones.size(); stifness <= stiffnessConstant;)
	{
        //1. attract connected zones
		attractConnectedZones(forces, distances);
		for(const auto & zone : forces)
		{
			zone.first->setCenter (zone.first->getCenter() + zone.second);
			totalForces[zone.first] = zone.second; //override
		}

        //2. separate overlapping zones
		separateOverlappingZones(forces, overlaps);
		for(const auto & zone : forces)
		{
			zone.first->setCenter (zone.first->getCenter() + zone.second);
			totalForces[zone.first] += zone.second; //accumulate
		}

        bool improved = evaluateZones(distances, overlaps, bestSolution);


		if (!improved)
		{
			//Only cool down if we didn't see any improvement
			stifness *= stiffnessIncreaseFactor;
		}

        std::cerr << "Total distance: " << bestTotalDistance << " Total overlap: " << bestTotalOverlap << " Improvement: " << improved << "\n";

    }




    for(auto& zone : zones) {
        auto realCoords = getRealCoords(bestSolution[zone.second]);
        zone.second->setPosition(realCoords);
    }
}

void ZonePlacer::attractConnectedZones(TForceVector &forces, TDistanceVector &distances) {
    auto zones = map.getZones();

    for(const auto & zone : zones)
	{
		float3 forceVector(0, 0, 0);
		float3 pos = zone.second->getCenter();
		float totalDistance = 0;

        for(auto c : temp.getZonesI()[zone.first]->getConnections()) {
            if (c.getZoneA() == c.getZoneB())
			{
				continue;
			}

            auto otherZone = zones[c.getOtherZone(zone.second->getId())];
			float3 otherZoneCenter = otherZone->getCenter();
			auto distance = pos.distance2DSQ(otherZoneCenter);
			
			forceVector += (otherZoneCenter - pos) * distance * gravityConstant; //positive value

			//Attract zone centers always

			float minDistance = 0;

            float mapSize = sqrt(mapWidth * mapHeight);

			
			minDistance = (zone.second->getSize() + otherZone->getSize()) / mapSize; //scale down to (0,1) coordinates

            std::cerr << "Zone id: " << zone.first << " otherZoneID " << c.getOtherZone(zone.second->getId()) << " Distance: " << distance << " Min distance: " << minDistance << "\n";

			if (distance > minDistance)
				totalDistance += (distance - minDistance);
        }

        std::cerr << "Zone id: " << zone.first << " Total distance: " << totalDistance << "\n";
        distances[zone.second] = totalDistance;
        forceVector.z = 0; //operator - doesn't preserve z coordinate :/
        forces[zone.second] = forceVector;
    }

}   

void ZonePlacer::separateOverlappingZones(TForceVector &forces, TDistanceVector &overlaps) {
    auto zones = map.getZones();

    for(const auto & zone : zones)
    {
        float3 forceVector(0, 0, 0);
        float3 pos = zone.second->getCenter();
        float overlap = 0;

        for(auto& otherZone : zones) {
            if (zone.first == otherZone.first)
            {
                continue;
            }

            float3 otherZoneCenter = otherZone.second->getCenter();
            auto distance = pos.distance2DSQ(otherZoneCenter);
            float minDistance = (zone.second->getSize() + otherZone.second->getSize()) / sqrt(mapWidth * mapHeight); //scale down to (0,1) coordinates

            if (distance < minDistance)
            {

                float3 localForce = (((otherZoneCenter - pos)*(minDistance / (distance ? distance : 1e-3f))) / (distance ? distance * distance : 1e-6f)) * stifness;
				//negative value
				// localForce *= scaleForceBetweenZones(zone.second, otherZone.second);
				forceVector -= localForce * (DistancesBetweenZones[zone.second->getId()][otherZone.second->getId()] / 2.0f);
				overlap += (minDistance - distance); //overlapping of small zones hurts us more
            }
        }


        //move zones away from boundaries
		//do not scale boundary distance - zones tend to get squashed
		float size = zone.second->getSize() / sqrt(mapWidth * mapHeight);

		auto pushAwayFromBoundary = [&forceVector, pos, size, &overlap, this](float x, float y)
		{
			float3 boundary = float3(x, y, pos.z);
			auto distance = static_cast<float>(pos.distance2DSQ(boundary));
			overlap += std::max<float>(0, distance - size); //check if we're closer to map boundary than value of zone size
			forceVector -= (boundary - pos) * (size - distance) / (distance ? distance * distance : 1e-6f) * stifness; //negative value
		};
		if (pos.x < size)
		{
			pushAwayFromBoundary(0, pos.y);
		}
		if (pos.x > 1 - size)
		{
			pushAwayFromBoundary(1, pos.y);
		}
		if (pos.y < size)
		{
			pushAwayFromBoundary(pos.x, 0);
		}
		if (pos.y > 1 - size)
		{
			pushAwayFromBoundary(pos.x, 1);
		}


        overlaps[zone.second] = overlap;
		forceVector.z = 0; //operator - doesn't preserve z coordinate :/
		forces[zone.second] = forceVector;
    }
}

bool ZonePlacer::evaluateZones(TDistanceVector &distances, TDistanceVector &overlaps, TBestSolution &bestSolution) {
    auto zones = map.getZones();

    bool improvement = false;

    float totalDistance = 0;
    float totalOverlap = 0;
    for (const auto& zone : distances) //find most misplaced zone
    {
        totalDistance += zone.second;
        float overlap = overlaps[zone.first];
        totalOverlap += overlap;
    }
    //check fitness function
    if ((totalDistance + 1) * (totalOverlap + 1) < (bestTotalDistance + 1) * (bestTotalOverlap + 1))
    {
        //multiplication is better for auto-scaling, but stops working if one factor is 0
        improvement = true;
    }

    //Save best solution
    if (improvement)
    {
        bestTotalDistance = totalDistance;
        bestTotalOverlap = totalOverlap;

        for (const auto& zone : zones)
            bestSolution[zone.second] = zone.second->getCenter();
    }

    return improvement;
    
}

void ZonePlacer::prepareZones() {
    auto zones = map.getZones();

    float totalSize[2]; // 0 - above, 1 - underground 
    totalSize[0] = 0;
    totalSize[1] = 0; 

    for(auto& zone : zones) {
        auto zoneSize = zone.second->getSize();

        totalSize[0] += zoneSize * zoneSize;
    }

    /*
	prescale zones

	formula: sum((prescaler*n)^2)*pi = WH

	prescaler = sqrt((WH)/(sum(n^2)*pi))
	*/

    float prescaler = std::sqrt((mapWidth * mapHeight) / (totalSize[0] * PI_CONSTANT));

    for(auto& zone : zones) {
        auto zoneSize = zone.second->getSize();
        zoneSize = zoneSize * prescaler;
        zone.second->setSize(zoneSize);
    }

}

void ZonePlacer::moveToCenterOfTheMass() {

    auto zones = map.getZones();

    

    for (auto& zone : zones) {

        int3 total(0, 0, 0);

        int totalTiles = 0;
        for(int x = 0; x < mapWidth; x++) {
            for(int y = 0; y < mapHeight; y++) {
                auto TilePtr = map.getTile(x, y);
                if(TilePtr && TilePtr->getZoneId() == zone.first) {
                    total += int3(x, y, 0);
                    totalTiles++;
                }
            }
        }

        assert(totalTiles);

        int3 newPos = int3(total.x / totalTiles, total.y / totalTiles, total.z / totalTiles);

        zone.second->setPosition(newPos);
        zone.second->setCenter(float3(float(newPos.x) / mapWidth, float(newPos.y) / mapHeight, newPos.z));

	}
}

void ZonePlacer::paintTiles() {
    std::cerr << "Painting tiles\n";
    auto zones = map.getZones();


    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {

            float minDist = 1000000;
            int minDistZone = -1;
            for(auto& zone : zones) {
                
                auto zonePos = zone.second->getPosition();
                float3 zonePosF = float3(zonePos.x, zonePos.y, 0);
                float3 tilePos = float3(x, y, 0);

                float dist = zonePosF.distance2DSQ(tilePos);
                
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

    std::cerr << "BEFORE MOVING TO CENTER OF MASS\n";
    for(auto& zone : zones) {
        auto zonePos = zone.second->getPosition();
        std::cerr << "Zone id: " << zone.first << " Position: " << zonePos.x << " " << zonePos.y << " " << zonePos.z << "\n";
    }

    moveToCenterOfTheMass();    

    std::cerr << "AFTER MOVING TO CENTER OF MASS\n";

    for(auto& zone : zones) {
        auto zonePos = zone.second->getPosition();
        std::cerr << "Zone id: " << zone.first << " Position: " << zonePos.x << " " << zonePos.y << " " << zonePos.z << "\n";
    }

    PenroseTiling penrose;
    auto vertices = penrose.generatePenroseTiling(zones.size(), rng);

    std::vector<Point2D> verticesVector(vertices.begin(), vertices.end());

    for(auto& vertice : verticesVector) {

        vertice.setX(vertice.getX() * mapWidth);
        vertice.setY(vertice.getY() * mapHeight);
        
    }

    std::vector<std::pair<Point2D, std::shared_ptr<Zone>>> zoneVertices;
    

    for(auto& vertice : verticesVector) {

        float minDist = 1000000;
        int minDistZone = -1;
        for(auto& zone : zones) {
            int3 zonePosI = zone.second->getPosition();

            float3 zonePos = float3(zonePosI.x, zonePosI.y, 0);
            float3 verticePos = float3(vertice.getX(), vertice.getY(), 0);

            float dist = zonePos.distance2DSQ(verticePos);
            
            if(dist < minDist) {
                minDist = dist;
                minDistZone = zone.first;
            }
        }

        zoneVertices.push_back({vertice, zones[minDistZone]});

    }

    if(debug){
        for(auto verticeZone : zoneVertices) {
            std::cerr << "Vertice: " << verticeZone.first.toString() << " Zone: " << verticeZone.second->getId() << "\n";
        }
    }


    for(int x = 0; x < mapWidth; x++) {
        for(int y = 0; y < mapHeight; y++) {

            float minDist = 1000000;
            int minDistZone = -1;
            for(auto& verticeZone : zoneVertices) {
                auto vertice = verticeZone.first;
                auto zone = verticeZone.second;

                float3 verticePos = float3(vertice.getX(), vertice.getY(), 0);
                float3 tilePos = float3(x, y, 0);

                float dist = verticePos.distance2DSQ(tilePos);
                
                if(dist < minDist) {
                    minDist = dist;
                    minDistZone = zone->getId();
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
    const int MAX_TRIES = 1000;
    auto key = std::make_pair(std::min(zoneA, zoneB), std::max(zoneA, zoneB));
    auto& points = connections[key];
    
    if (points.empty()) return {-1, -1};
    int random_ind = 0;

    for (int noTry = 0; noTry <= MAX_TRIES; noTry++){
        random_ind = rng->nextInt(0, points.size() - 1);
        int x = points[random_ind].first;
        int y = points[random_ind].second;

        auto TilePtr = map.getTile(x, y);
        if (TilePtr && TilePtr->getNeighbourZones() == 1 && countEdges(x, y) >= 3) {
            break;
        }
    }

    return points[random_ind];
}

int ZonePlacer::countEdges(int x, int y){
    int no = 0;

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];

        auto NeighborTilePtr = map.getTile(nx, ny);

        if(NeighborTilePtr && NeighborTilePtr->getIsEdge()){
            no++;
        }
    }

    return no;
}

void ZonePlacer::addConnectionPoint(int zoneA, int zoneB, int x, int y) {
    auto key = std::make_pair(std::min(zoneA, zoneB), std::max(zoneA, zoneB));
    connections[key].emplace_back(x, y);
}

bool ZonePlacer::isMapEdge(int x, int y){
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];

        auto NeighborTilePtr = map.getTile(nx, ny);

        if(!NeighborTilePtr){
            return true;
        }
    }

    return false;
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

                    if(!isMapEdge(x, y)){
                        addConnectionPoint(currentZoneId, NeighborTilePtr->getZoneId(), x, y);
                        NeighborTilePtr->setNeighbourZones(currentZoneId);
                        TilePtr->setNeighbourZones(NeighborTilePtr->getZoneId());
                    }
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

            std::cerr << e.first << " " << c.getZoneA() << " " << c.getZoneB() << "\n";
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
                        TilePtr->setIsGate(true);
                        TilePtr->setIsMiddleGate(false);
                    } 
                    if(nx == x && ny == y){
                        TilePtr->setIsMiddleGate(true);
                    }
                }
            }
            
            if(debug){
                std::cerr << "Modified tiles around connection point (" << x << ", " << y << ") for zones " 
                        << zone1 << " -> " << zone2 << "\n";
            }
        }
    }

    // if(debug){
        // std::cerr << "SIZE:\n";
        // for (int y = 0; y < mapHeight; y++)
        // {
        //     for (int x = 0; x < mapWidth; x++) {
        //         auto TilePtr = map.getTile(x, y);

        //         int currentZoneId = TilePtr->getNeighbourZones();

        //         std::cerr << currentZoneId << " ";
        //     }
        //     std::cerr << "\n";
        // }

        std::cerr << "\n\nGATES\n";

        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                auto TilePtr = map.getTile(x, y);

                if(TilePtr->getIsGate() && !TilePtr->getIsMiddleGate() && !TilePtr->getIsEdge()){
                    std::cerr << "G ";
                } else if(!TilePtr->getIsGate() && TilePtr->getIsEdge()){
                    std::cerr << "E ";
                } else if(TilePtr->getIsGate() && TilePtr->getIsMiddleGate()){
                    std::cerr << "M ";
                } else {
                    std::cerr << ". ";
                }
            }
            std::cerr << "\n";
        }
    // }
    std::cerr << "Finished modifying connection tiles\n";
}
