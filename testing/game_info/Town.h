#include "./Faction.h"

class Town {
public:
    Town();
    Town(Faction faction);

    void setFaction(Faction faction);
    
    Faction getFaction() const;




private:
    Faction faction;
};