#include <bits/stdc++.h>

// BASED ON https://heroes.thelazy.net/index.php/List_of_creatures

// Level 1 Creatures
static std::string basic_lvl1[] = {
    "PIKEMAN", "CENTAUR", "GREMLIN", "IMP", "SKELETON", "TROGLODYTE", "GOBLIN", "GNOLL"
};

// Level 1+ Creatures
static std::string upgraded_lvl1[] = {
    "HALBERDIER", "CENTAUR_CAPTAIN", "MASTER_GREMLIN", "FAMILIAR", "SKELETON_WARRIOR", "INFERNAL_TROGLODYTE", 
    "HOBGOBLIN", "GNOLL_MARAUDER"
};

// Level 2 Creatures
static std::string basic_lvl2[] = {
    "ARCHER", "DWARF", "STONE_GARGOYLE", "GOG", "WALKING_DEAD", "HARPY", "WOLF_RIDER", "LIZARDMAN", "AIR_ELEMENTAL"
};

// Level 2+ Creatures
static std::string upgraded_lvl2[] = {
"MARKSMAN", "BATTLE_DWARF", "OBSIDIAN_GARGOYLE", "MAGOG", "ZOMBIE", "HARPY_HAG", "WOLF_RAIDER", 
"LIZARD_WARRIOR" // "STORM_ELEMENTAL"
};

// Level 3 Creatures
static std::string basic_lvl3[] = {
    "GRIFFIN", "WOOD_ELF", "STONE_GOLEM", "HELL_HOUND", "WIGHT", "BEHOLDER", "ORC", "SERPENT_FLY", "WATER_ELEMENTAL"
};

// Level 3+ Creatures, for some reason api can't place iron golem:(
static std::string upgraded_lvl3[] = { 
    "ROYAL_GRIFFIN", "GRAND_ELF", "CERBERUS", "WRAITH", "EVIL_EYE", "ORC_CHIEFTAIN", 
    "DRAGON_FLY" //, "IRON_GOLEM" //, "ICE_ELEMENTAL"
};

// Level 4 Creatures
static std::string basic_lvl4[] = {
    "SWORDSMAN", "PEGASUS", "MAGE", "DEMON", "VAMPIRE", "MEDUSA", "OGRE", "BASILISK", "FIRE_ELEMENTAL"
};

// Level 4+ Creatures
static std::string upgraded_lvl4[] = {
    "CRUSADER", "SILVER_PEGASUS", "ARCH_MAGE", "HORNED_DEMON", "VAMPIRE_LORD", "MEDUSA_QUEEN", 
    "OGRE_MAGE", "GREATER_BASILISK" // "ENERGY_ELEMENTAL"
};

// Level 5 Creatures
static std::string basic_lvl5[] = {
    "MONK", "DENDROID_GUARD", "GENIE", "PIT_FIEND", "LICH", "MINOTAUR", "ROC", "GORGON", "EARTH_ELEMENTAL"
};

// Level 5+ Creatures
static std::string upgraded_lvl5[] = {
    "ZEALOT", "DENDROID_SOLDIER", "MASTER_GENIE", "PIT_LORD", "POWER_LICH", "MINOTAUR_KING", 
    "THUNDERBIRD", "MIGHTY_GORGON" // "MAGMA_ELEMENTAL"
};

// Level 6 Creatures
static std::string basic_lvl6[] = {
    "CAVALIER", "UNICORN", "NAGA", "EFREETI", "BLACK_KNIGHT", "MANTICORE", "CYCLOPS", "WYVERN" // "PSYCHIC_ELEMENTAL"
};

// Level 6+ Creatures
static std::string upgraded_lvl6[] = {
    "CHAMPION", "WAR_UNICORN", "NAGA_QUEEN", "EFREET_SULTAN", "DREAD_KNIGHT", "SCORPICORE", 
    "CYCLOPS_KING", "WYVERN_MONARCH" // "MAGIC_ELEMENTAL"
};

// Level 7 Creatures
static std::string basic_lvl7[] = {
    "ANGEL", "GREEN_DRAGON", "GIANT", "DEVIL", "BONE_DRAGON", "RED_DRAGON", "BEHEMOTH", "HYDRA" // "FIREBIRD"
};

// Level 7+ Creatures
static std::string upgraded_lvl7[] = {
    "ARCHANGEL", "GOLD_DRAGON", "TITAN", "ARCH_DEVIL", "GHOST_DRAGON", "BLACK_DRAGON", 
    "ANCIENT_BEHEMOTH", "CHAOS_HYDRA" // "PHOENIX"
};