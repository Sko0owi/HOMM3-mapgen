#pragma once

#include "./Faction.h"
#include "./Object.h"

class Town : public Object {
public:
    Town();
    Town(Faction faction);

    void setFaction(Faction faction);
    void setOwner(int owner);

    int getOwner() const;
    Faction getFaction() const;

private:
    Faction faction;

    int owner;
};