#include "./TreasureType.h"
#include "../types/int3.h"

std::string treasureTypeToString(TreasureType treasureType) {
    switch (treasureType) {
        case TreasureType::RESOURCE_GOLD:
            return "RESOURCE_GOLD";
        case TreasureType::RESOURCE_CRYSTAL:
            return "RESOURCE_CRYSTAL";
        case TreasureType::RESOURCE_GEMS:
            return "RESOURCE_GEMS";
        case TreasureType::RESOURCE_MERCURY:
            return "RESOURCE_MERCURY";
        case TreasureType::RESOURCE_ORE:
            return "RESOURCE_ORE";
        case TreasureType::RESOURCE_SULFUR:
            return "RESOURCE_SULFUR";
        case TreasureType::RESOURCE_WOOD:
            return "RESOURCE_WOOD";
        case TreasureType::ARTIFACT_LADYBIRD_OF_LUCK:
            return "ARTIFACT_LADYBIRD_OF_LUCK";
        case TreasureType::ARTIFACT_STATESMANS_MEDAL:
            return "ARTIFACT_STATESMANS_MEDAL";
        case TreasureType::ARTIFACT_CLOVER_OF_FORTUNE:
            return "ARTIFACT_CLOVER_OF_FORTUNE";
        case TreasureType::ARTIFACT_EMBLEM_OF_COGNIZANCE:
            return "ARTIFACT_EMBLEM_OF_COGNIZANCE";
        case TreasureType::ARTIFACT_RING_OF_VITALITY:   
            return "ARTIFACT_RING_OF_VITALITY";
        case TreasureType::ARTIFACT_TALISMAN_OF_MANA:
            return "ARTIFACT_TALISMAN_OF_MANA";
        case TreasureType::ARTIFACT_SPYGLASS:   
            return "ARTIFACT_SPYGLASS";
        case TreasureType::ARTIFACT_STOIC_WATCHMAN:
            return "ARTIFACT_STOIC_WATCHMAN";
        case TreasureType::ARTIFACT_SPECULUM:   
            return "ARTIFACT_SPECULUM";
        case TreasureType::ARTIFACT_CHARM_OF_MANA:  
            return "ARTIFACT_CHARM_OF_MANA";
        case TreasureType::ARTIFACT_RING_OF_LIFE:
            return "ARTIFACT_RING_OF_LIFE";
        case TreasureType::ARTIFACT_BADGE_OF_COURAGE:   
            return "ARTIFACT_BADGE_OF_COURAGE";
        case TreasureType::ARTIFACT_SHIELD_OF_THE_DWARVEN_LORDS:    
            return "ARTIFACT_SHIELD_OF_THE_DWARVEN_LORDS";
        case TreasureType::ARTIFACT_BUCKLER_OF_THE_GNOLL_KING:
            return "ARTIFACT_BUCKLER_OF_THE_GNOLL_KING";
        case TreasureType::ARTIFACT_GREATER_GNOLLS_FLAIL:
            return "ARTIFACT_GREATER_GNOLLS_FLAIL";
        case TreasureType::ARTIFACT_DRAGON_WING_TABARD:
            return "ARTIFACT_DRAGON_WING_TABARD";
        case TreasureType::ARTIFACT_RING_OF_THE_WAYFARER:
            return "ARTIFACT_RING_OF_THE_WAYFARER";
        case TreasureType::ARTIFACT_RING_OF_INFINITE_GEMS:
            return "ARTIFACT_RING_OF_INFINITE_GEMS";
        case TreasureType::ARTIFACT_EQUESTRIANS_GLOVES:
            return "ARTIFACT_EQUESTRIANS_GLOVES";
        case TreasureType::ARTIFACT_SANDALS_OF_THE_SAINT:   
            return "ARTIFACT_SANDALS_OF_THE_SAINT";
        case TreasureType::ARTIFACT_PENDANT_OF_LIFE:
            return "ARTIFACT_PENDANT_OF_LIFE";
        case TreasureType::ARTIFACT_PENDANT_OF_COURAGE:
            return "ARTIFACT_PENDANT_OF_COURAGE";
        case TreasureType::ARTIFACT_PENDANT_OF_FREE_WILL:
            return "ARTIFACT_PENDANT_OF_FREE_WILL";
        case TreasureType::ARTIFACT_PENDANT_OF_HOLINESS:
            return "ARTIFACT_PENDANT_OF_HOLINESS";
        case TreasureType::ARTIFACT_PENDANT_OF_TOTAL_RECALL:
            return "ARTIFACT_PENDANT_OF_TOTAL_RECALL";
        case TreasureType::ARTIFACT_CAPE_OF_CONJURING:  
            return "ARTIFACT_CAPE_OF_CONJURING";
        case TreasureType::ARTIFACT_BOWSTRING_OF_THE_UNICORNS_MANE:  
            return "ARTIFACT_BOWSTRING_OF_THE_UNICORNS_MANE";  
        case TreasureType::ARTIFACT_BOOTS_OF_POLARITY:
            return "ARTIFACT_BOOTS_OF_POLARITY";
        case TreasureType::ARTIFACT_NECKLACE_OF_SWIFTNESS:
            return "ARTIFACT_NECKLACE_OF_SWIFTNESS";
        case TreasureType::ARTIFACT_VIAL_OF_LIFEBLOOD:  
            return "ARTIFACT_VIAL_OF_LIFEBLOOD";
        case TreasureType::ARTIFACT_NECKLACE_OF_OCEAN_GUIDANCE:
            return "ARTIFACT_NECKLACE_OF_OCEAN_GUIDANCE";
        case TreasureType::ARTIFACT_BREASTPLATE_OF_PETRIFIED_WOOD:
            return "ARTIFACT_BREASTPLATE_OF_PETRIFIED_WOOD";
        case TreasureType::ARTIFACT_SURCOAT_OF_COUNTERPOISE:
            return "ARTIFACT_SURCOAT_OF_COUNTERPOISE";
        case TreasureType::ARTIFACT_HELM_OF_THE_ALABASTER_UNICORN:
            return "ARTIFACT_HELM_OF_THE_ALABASTER_UNICORN";
        case TreasureType::ARTIFACT_ARMOR_OF_WONDER:
            return "ARTIFACT_ARMOR_OF_WONDER";
        case TreasureType::ARTIFACT_GLYPH_OF_GALLANTRY:
            return "ARTIFACT_GLYPH_OF_GALLANTRY";
        case TreasureType::ARTIFACT_SENTINELS_SHIELD:
            return "ARTIFACT_SENTINELS_SHIELD";
        case TreasureType::ARTIFACT_DRAGON_SCALE_SHIELD:   
            return "ARTIFACT_DRAGON_SCALE_SHIELD";
        case TreasureType::ARTIFACT_ANGEL_FEATHER_ARROWS:
            return "ARTIFACT_ANGEL_FEATHER_ARROWS";
    
        default:
            return "RESOURCE_GOLD";
    
    }
    return "RESOURCE_GOLD";
}

TreasureType stringToTreasureType(const std::string treasureType) {
    if (treasureType == "RESOURCE_GOLD") return TreasureType::RESOURCE_GOLD;
    if (treasureType == "RESOURCE_CRYSTAL") return TreasureType::RESOURCE_CRYSTAL;
    if (treasureType == "RESOURCE_GEMS") return TreasureType::RESOURCE_GEMS;
    if (treasureType == "RESOURCE_MERCURY") return TreasureType::RESOURCE_MERCURY;
    if (treasureType == "RESOURCE_ORE") return TreasureType::RESOURCE_ORE;
    if (treasureType == "RESOURCE_SULFUR") return TreasureType::RESOURCE_SULFUR;
    if (treasureType == "RESOURCE_WOOD") return TreasureType::RESOURCE_WOOD;
    if (treasureType == "ARTIFACT_LADYBIRD_OF_LUCK") return TreasureType::ARTIFACT_LADYBIRD_OF_LUCK;
    if (treasureType == "ARTIFACT_STATESMANS_MEDAL") return TreasureType::ARTIFACT_STATESMANS_MEDAL;
    if (treasureType == "ARTIFACT_CLOVER_OF_FORTUNE") return TreasureType::ARTIFACT_CLOVER_OF_FORTUNE;
    if (treasureType == "ARTIFACT_EMBLEM_OF_COGNIZANCE") return TreasureType::ARTIFACT_EMBLEM_OF_COGNIZANCE;
    if (treasureType == "ARTIFACT_RING_OF_VITALITY") return TreasureType::ARTIFACT_RING_OF_VITALITY;
    if (treasureType == "ARTIFACT_TALISMAN_OF_MANA") return TreasureType::ARTIFACT_TALISMAN_OF_MANA;
    if (treasureType == "ARTIFACT_SPYGLASS") return TreasureType::ARTIFACT_SPYGLASS;
    if (treasureType == "ARTIFACT_STOIC_WATCHMAN") return TreasureType::ARTIFACT_STOIC_WATCHMAN;
    if (treasureType == "ARTIFACT_SPECULUM") return TreasureType::ARTIFACT_SPECULUM;
    if (treasureType == "ARTIFACT_CHARM_OF_MANA") return TreasureType::ARTIFACT_CHARM_OF_MANA;
    if (treasureType == "ARTIFACT_RING_OF_LIFE") return TreasureType::ARTIFACT_RING_OF_LIFE;
    if (treasureType == "ARTIFACT_BADGE_OF_COURAGE") return TreasureType::ARTIFACT_BADGE_OF_COURAGE;
    if (treasureType == "ARTIFACT_SHIELD_OF_THE_DWARVEN_LORDS") return TreasureType::ARTIFACT_SHIELD_OF_THE_DWARVEN_LORDS;
    if (treasureType == "ARTIFACT_BUCKLER_OF_THE_GNOLL_KING") return TreasureType::ARTIFACT_BUCKLER_OF_THE_GNOLL_KING;
    if (treasureType == "ARTIFACT_GREATER_GNOLLS_FLAIL") return TreasureType::ARTIFACT_GREATER_GNOLLS_FLAIL;
    if (treasureType == "ARTIFACT_DRAGON_WING_TABARD") return TreasureType::ARTIFACT_DRAGON_WING_TABARD;
    if (treasureType == "ARTIFACT_RING_OF_THE_WAYFARER") return TreasureType::ARTIFACT_RING_OF_THE_WAYFARER;
    if (treasureType == "ARTIFACT_RING_OF_INFINITE_GEMS") return TreasureType::ARTIFACT_RING_OF_INFINITE_GEMS;
    if (treasureType == "ARTIFACT_EQUESTRIANS_GLOVES") return TreasureType::ARTIFACT_EQUESTRIANS_GLOVES;
    if (treasureType == "ARTIFACT_SANDALS_OF_THE_SAINT") return TreasureType::ARTIFACT_SANDALS_OF_THE_SAINT;
    if (treasureType == "ARTIFACT_PENDANT_OF_LIFE") return TreasureType::ARTIFACT_PENDANT_OF_LIFE;
    if (treasureType == "ARTIFACT_PENDANT_OF_COURAGE") return TreasureType::ARTIFACT_PENDANT_OF_COURAGE;
    if (treasureType == "ARTIFACT_PENDANT_OF_FREE_WILL") return TreasureType::ARTIFACT_PENDANT_OF_FREE_WILL;
    if (treasureType == "ARTIFACT_PENDANT_OF_HOLINESS") return TreasureType::ARTIFACT_PENDANT_OF_HOLINESS;
    if (treasureType == "ARTIFACT_PENDANT_OF_TOTAL_RECALL") return TreasureType::ARTIFACT_PENDANT_OF_TOTAL_RECALL;
    if (treasureType == "ARTIFACT_CAPE_OF_CONJURING") return TreasureType::ARTIFACT_CAPE_OF_CONJURING;
    if (treasureType == "ARTIFACT_BOWSTRING_OF_THE_UNICORNS_MANE") return TreasureType::ARTIFACT_BOWSTRING_OF_THE_UNICORNS_MANE;
    if (treasureType == "ARTIFACT_BOOTS_OF_POLARITY") return TreasureType::ARTIFACT_BOOTS_OF_POLARITY;
    if (treasureType == "ARTIFACT_NECKLACE_OF_SWIFTNESS") return TreasureType::ARTIFACT_NECKLACE_OF_SWIFTNESS;
    if (treasureType == "ARTIFACT_VIAL_OF_LIFEBLOOD") return TreasureType::ARTIFACT_VIAL_OF_LIFEBLOOD;
    if (treasureType == "ARTIFACT_NECKLACE_OF_OCEAN_GUIDANCE") return TreasureType::ARTIFACT_NECKLACE_OF_OCEAN_GUIDANCE;
    if (treasureType == "ARTIFACT_BREASTPLATE_OF_PETRIFIED_WOOD") return TreasureType::ARTIFACT_BREASTPLATE_OF_PETRIFIED_WOOD;
    if (treasureType == "ARTIFACT_SURCOAT_OF_COUNTERPOISE") return TreasureType::ARTIFACT_SURCOAT_OF_COUNTERPOISE;
    if (treasureType == "ARTIFACT_HELM_OF_THE_ALABASTER_UNICORN") return TreasureType::ARTIFACT_HELM_OF_THE_ALABASTER_UNICORN;
    if (treasureType == "ARTIFACT_ARMOR_OF_WONDER") return TreasureType::ARTIFACT_ARMOR_OF_WONDER;
    if (treasureType == "ARTIFACT_GLYPH_OF_GALLANTRY") return TreasureType::ARTIFACT_GLYPH_OF_GALLANTRY;
    if (treasureType == "ARTIFACT_SENTINELS_SHIELD") return TreasureType::ARTIFACT_SENTINELS_SHIELD;
    if (treasureType == "ARTIFACT_DRAGON_SCALE_SHIELD") return TreasureType::ARTIFACT_DRAGON_SCALE_SHIELD;
    if (treasureType == "ARTIFACT_ANGEL_FEATHER_ARROWS") return TreasureType::ARTIFACT_ANGEL_FEATHER_ARROWS;

    return TreasureType::RESOURCE_GOLD;
}


int3 getTreasureSize(TreasureType TreasureType) {
    switch(TreasureType) {
        case TreasureType::RESOURCE_GOLD:
            return int3(1, 1, 1);
        default:
            return int3(1, 1, 1);
    }
    return int3(1,1,1);
}