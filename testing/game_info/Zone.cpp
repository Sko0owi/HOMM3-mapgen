#include "./Zone.h"
#include "../global/Global.h"
#include "./Faction.h"


Zone::Zone() {
    id = -1;
    position = int3(0,0,0);
    center = float3(0,0,0);
    owner = faction::NEUTRAL;
}

Zone::Zone(i32 Id) {
    id = Id;
    position = int3(0,0,0);
    center = float3(0,0,0);
    owner = faction::NEUTRAL;
}

Zone::Zone(i32 Id, float3 center, faction owner) {
    id = Id;
    this->center = center;
    this->owner = owner;
}

void Zone::setPosition(int3 pos){
    position = pos;
}
void Zone::setCenter(float3 cent){
    center = cent;
}
void Zone::setOwner(faction fac){
    owner = fac;
}
int3 Zone::getPosition(){
    return position;
}
float3 Zone::getCenter(){
    return center;
}
faction Zone::getOwner(){
    return owner;
}

int Zone::getId(){
    return id;
}

void Zone::printZone(){
    std::cerr << "Zone id: " << id << "\n";
    std::cerr << "Zone position: " << position.x << " " << position.y << " " << position.z << "\n";
    std::cerr << "Zone center: " << center.x << " " << center.y << " " << center.z << "\n";
    std::cerr << "Zone owner: " << factionToString(owner) << "\n";
}