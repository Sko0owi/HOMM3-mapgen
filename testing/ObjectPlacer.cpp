#include "./ObjectPlacer.h"

#include "./Map.h"
#include "./Template.h"
#include "./global/Random.h"
#include "./game_info/Town.h"
#include "./game_info/Zone.h"
#include "./game_info/Object.h"

ObjectPlacer::ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng) : map(map), temp(temp), rng(rng) {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();
}

void ObjectPlacer::placeObjects() {
    std::cerr << "Placing objects\n";

    placeTowns();
}

void ObjectPlacer::placeTowns() {
    std::cerr << "Placing towns\n";

    auto zones = map.getZones();

    for(auto& zone : zones) {
        auto zoneId = zone.first;
        auto zonePtr = zone.second;

        auto zoneI = temp.getZonesI()[zoneId];

        for (auto& town : zoneI->getTowns()) {

            town.setPosition(zonePtr->getPosition());

            map.addTown(town);
        }
        
    }
}
