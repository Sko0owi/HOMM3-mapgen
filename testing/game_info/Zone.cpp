#include "./Zone.h"

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