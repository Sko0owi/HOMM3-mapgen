#include "./TreasuresInfo.h"

TreasuresInfo::TreasuresInfo() {
}

TreasuresInfo::TreasuresInfo(ZoneRichness richness) {
    switch(richness) {
        case ZoneRichness::Low:
            frequency[0] = 60;
            frequency[1] = 30;
            frequency[2] = 10;
            break;
        case ZoneRichness::Normal:
            frequency[0] = 40;
            frequency[1] = 40;
            frequency[2] = 20;
            break;
        case ZoneRichness::Rich:
            frequency[0] = 20;
            frequency[1] = 40;
            frequency[2] = 40;
            break;
    }
}

int TreasuresInfo::getValueMin(ZoneRichness richness) const {
    return valueMin[static_cast<int>(richness)];
}

int TreasuresInfo::getValueMax(ZoneRichness richness) const {
    return valueMax[static_cast<int>(richness)];
}

int TreasuresInfo::getFrequency(ZoneRichness richness) const {
    return frequency[static_cast<int>(richness)];
}