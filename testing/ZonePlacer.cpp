#include "./ZonePlacer.h"

void ZonePlacer::generateZones() {
    placeOnGrid();
    placeZones();
}

void ZonePlacer::placeOnGrid() {
    mapWidth = 36;
    mapHeight = 36;
}

void ZonePlacer::placeZones() {
    std::cerr << "Placing zones on grid\n";
}
