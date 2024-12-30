#include "../types/int3.h"
#include "../types/float3.h"

enum class faction;

class Zone
{
public:
Zone();
Zone(i32 Id);
Zone(i32 Id, float3 center, faction owner);

void setPosition(int3 pos);
void setCenter(float3 cent);
void setOwner(faction fac);


int3 getPosition();
float3 getCenter();
faction getOwner();
int getId();


void printZone();


private:
    int3 position;
    float3 center;
    faction owner;
    i32 id;

};
