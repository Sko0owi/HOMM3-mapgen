
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

Difficulty stringToDifficulty(const std::string &difficulty) {
    if (difficulty == "Beginner") return Difficulty::Beginner;
    if (difficulty == "Easy") return Difficulty::Easy;
    if (difficulty == "Normal") return Difficulty::Normal;
    if (difficulty == "Hard") return Difficulty::Hard;
    if (difficulty == "Expert") return Difficulty::Expert;
    if (difficulty == "Impossible") return Difficulty::Impossible;
    return Difficulty::Invalid; 
}

class GuardPlacer {
public:
    GuardPlacer(std::ofstream &luaFile, Map &map, TemplateInfo &temp, RNG *rng);
    void placeGuards();

    std::string getZoneDifficulty(int zoneId);
    std::pair<int, int> getQuantityRange(const std::string &difficulty);
    bool doesNotGrow(const std::string &difficulty);
    bool neverFlies(const std::string &difficulty);
    std::pair<double, double> getGuardLevel(const std::string &difficulty);
    std::pair<double, double> getBorderGuardLevel(const std::string &difficulty);

private:
    std::ofstream &luaFile;
    Map &map;
    TemplateInfo &temp;
    RNG *rng;
};
