#include "./ObjectPlacer.h"

#include "./Map.h"
#include "./Template.h"
#include "./Object.h"
#include "./global/Random.h"

ObjectPlacer::ObjectPlacer(Map & map, TemplateInfo & temp, RNG *rng) : map(map), temp(temp), rng(rng) {
    mapWidth = map.getWidth();
    mapHeight = map.getHeight();
}

void ObjectPlacer::placeObjects() {
    std::cerr << "Placing objects\n";

    
}
