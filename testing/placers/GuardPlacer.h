
#include <bits/stdc++.h>


class Map;
class TemplateInfo;
class RNG;

enum class Difficulty {
    Beginner,
    Easy,
    Normal,
    Hard,
    Expert,
    Impossible,
    Invalid
};

class GuardPlacer {
public:
    GuardPlacer(std::ofstream &luaFile, Map &map, TemplateInfo &temp, RNG *rng);
    void placeGuards();

    std::string getZoneDifficulty(int zoneId);
    std::pair<int, int> getQuantityRange(Difficulty diff);
    bool doesNotGrow(Difficulty diff);
    bool neverFlies(Difficulty diff);
    std::pair<double, double> getGuardLevel(Difficulty diff);
    std::pair<double, double> getBorderGuardLevel(Difficulty diff);
    std::string getDisposition(Difficulty diff);

    static Difficulty stringToDifficulty(const std::string &difficulty);

private:
    std::ofstream &luaFile;
    Map &map;
    TemplateInfo &temp;
    RNG *rng;
};