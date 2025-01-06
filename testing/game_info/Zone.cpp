#include "./Zone.h"
#include "../global/Global.h"
#include "./Terrain.h"
#include "./Town.h"

Zone::Zone() {
    id = -1;
    position = int3(0,0,0);
    center = float3(0,0,0);
}

Zone::Zone(i32 Id) {
    id = Id;
    position = int3(0,0,0);
    center = float3(0,0,0);
}

Zone::Zone(i32 Id, float3 center) {
    id = Id;
    this->center = center;
}

void Zone::setPosition(int3 pos){
    position = pos;
}
void Zone::setCenter(float3 cent){
    center = cent;
}
void Zone::setTerrain(Terrain ter){
    terrain = ter;
}
void Zone::setOwnerId(i32 ownerId){
    this->ownerId = ownerId;
}

void Zone::setSize(i32 size){
    this->size = size;
}

int3 Zone::getPosition(){
    return position;
}
float3 Zone::getCenter(){
    return center;
}
Terrain Zone::getTerrain(){
    return terrain;
}

int Zone::getId(){
    return id;
}
int Zone::getOwnerId(){
    return ownerId;
}

int Zone::getSize(){
    return size;
}



void Zone::printZone(){
    std::cerr << "Zone id: " << id << "\n";
    std::cerr << "Zone position: " << position.x << " " << position.y << " " << position.z << "\n";
    std::cerr << "Zone center: " << center.x << " " << center.y << " " << center.z << "\n";
    std::cerr << "Zone terrain: " << terrainToString(terrain) << "\n";
    std::cerr << "Zone owner: " << ownerId << "\n";
}