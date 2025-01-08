#pragma once

#include "../game_info/Faction.h"


class TownInfo {
public:
    TownInfo();
    TownInfo(Faction faction);
    TownInfo(Faction faction, int owner);
    TownInfo(Faction faction, int owner, int density, int minCount);

    void setFaction(Faction faction);
    void setOwner(int owner);
    void setDensity(int density);
    void setMinCount(int minCount);

    int getOwner() const;
    Faction getFaction() const;
    int getDensity() const;
    int getMinCount() const;

private:
    Faction faction;
    int density;
    int minCount;
    int owner;
};

