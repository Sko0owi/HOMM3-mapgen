
enum class ZoneRichness {
    Low,
    Normal,
    Rich
};

class TreasuresInfo {
public:
    TreasuresInfo();

    TreasuresInfo(ZoneRichness richness);

    int getValueMin(ZoneRichness richness) const;
    int getValueMax(ZoneRichness richness) const;

    int getFrequency(ZoneRichness richness) const;

private:

    int valueMin[3] = {500, 2001, 10001}; // 0 - low, 1 - normal, 2 - rich
    int valueMax[3] = {2000, 10000, 50000}; // 0 - low, 1 - normal, 2 - rich

    int frequency[3] = {60, 30, 10}; // 0 - low, 1 - normal, 2 - rich    

};