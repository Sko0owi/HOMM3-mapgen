#pragma once

#include "../types/int3.h"
#include "../types/float3.h"

enum class Terrain;
class Town;

class Zone
{
public:
Zone();
Zone(i32 Id);
Zone(i32 Id, float3 center);

void addTown(Town town);

void setPosition(int3 pos);
void setCenter(float3 cent);
void setTerrain(Terrain ter);
void setOwnerId(i32 ownerId);
void setSize(i32 size);

std::vector<Town>& getTowns();
int3 getPosition();
float3 getCenter();
Terrain getTerrain();
int getId();
int getOwnerId();
int getSize();


void printZone();


private:
    int3 position;
    float3 center;
    Terrain terrain;
    i32 id;

    i32 size = 1000;

    i32 ownerId;

    std::vector<Town> towns;

};
