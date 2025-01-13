#pragma once

#include "../gameInfo/Faction.h"


class TownInfo {
public:
    TownInfo();
    TownInfo(Faction faction);
    TownInfo(Faction faction, int owner);
    TownInfo(Faction faction, int owner, int minCount);

    void setFaction(Faction faction);
    void setOwner(int owner);
    void setMinCount(int minCount);

    int getOwner() const;
    Faction getFaction() const;
    int getMinCount() const;

private:
    Faction faction;
    int minCount;
    int owner;
};

