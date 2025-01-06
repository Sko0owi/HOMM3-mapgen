#pragma once

#include "./Faction.h"
#include "./Object.h"

class Town : Object {
public:
    Town();
    Town(Faction faction);

    void setFaction(Faction faction);
    
    Faction getFaction() const;




private:
    Faction faction;
};