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

void Zone::addObject(std::shared_ptr<Object> object){
    objects.push_back(object);
}

std::vector<std::shared_ptr<Object>> Zone::getObjects(){
    return objects;
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

int Zone::getSize(){
    return size;
}



void Zone::printZone(){
    std::cerr << "Zone id: " << id << "\n";
    std::cerr << "Zone position: " << position.x << " " << position.y << " " << position.z << "\n";
    std::cerr << "Zone center: " << center.x << " " << center.y << " " << center.z << "\n";
    std::cerr << "Zone terrain: " << terrainToString(terrain) << "\n";
    std::cerr << "Zone size: " << size << "\n";
}