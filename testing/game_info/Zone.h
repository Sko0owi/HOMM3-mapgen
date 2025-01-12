#pragma once

#include "../types/int3.h"
#include "../types/float3.h"

enum class Terrain;
class Town;
class Object;
class Creature;

class Zone
{
public:
Zone();
Zone(i32 Id);
Zone(i32 Id, float3 center);

void setPosition(int3 pos);
void setCenter(float3 cent);
void setTerrain(Terrain ter);
void setSize(i32 size);

int3 getPosition();
float3 getCenter();
Terrain getTerrain();
int getId();
int getSize();

void addObject(std::shared_ptr<Object> object);
std::vector<std::shared_ptr<Object>> getObjects();

void addCreature(std::shared_ptr<Creature> creature);
std::vector<std::shared_ptr<Creature>> getCreatures();


void printZone();


private:
    int3 position;
    float3 center;
    Terrain terrain;
    i32 id;

    i32 size = 1000;

    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Creature>> creatures;

};
