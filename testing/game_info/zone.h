
#include "../types/int3.h"
#include "../types/float3.h"
#include "./faction.h"

class Zone
{
public:
void setPosition(int3 pos);
void setCenter(float3 cent);
void setOwner(faction fac);
int3 getPosition();
float3 getCenter();
faction getOwner();


private:
    int3 position;
    float3 center;
    faction owner;
};
