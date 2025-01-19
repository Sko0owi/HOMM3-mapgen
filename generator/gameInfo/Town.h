#pragma once

#include "./Faction.h"
#include "./Object.h"
#include "../types/int3.h"

class TownInfo;

class Town : public Object {
public:
    Town();
    Town(Faction faction);
    Town(Faction faction, int owner, int3 pos, std::string name);
    Town(TownInfo townInfo);

    void setFaction(Faction faction);
    void setOwner(int owner);

    int getOwner() const;
    Faction getFaction() const;

private:
    Faction faction;

    int owner;
};