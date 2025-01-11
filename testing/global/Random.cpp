#include "./Random.h"
#include "./CreaturesConstants.h"

RNG::RNG() {
    gen = std::mt19937(rd());
}

RNG::RNG(int seed) {
    gen = std::mt19937(seed);
}

int RNG::nextInt() {
    std::uniform_int_distribution<> dis;
    return dis(gen);
}

int RNG::nextInt(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

float RNG::nextFloat() {
    std::uniform_real_distribution<> dis;
    return dis(gen);
}

float RNG::nextFloat(float min, float max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

double RNG::nextDouble() {
    std::uniform_real_distribution<> dis;
    return dis(gen);
}

double RNG::nextDouble(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

double RNG::nextDoubleRounded(double min, double max) {
        std::uniform_real_distribution<> dis(min, max);

        double value = dis(gen);
        return std::round(value * 2.0) / 2.0;
    }

bool RNG::nextBool() {
    std::bernoulli_distribution dis;
    return dis(gen);
}

bool RNG::nextBool(float probability) {
    std::bernoulli_distribution dis(probability);
    return dis(gen);
}

void RNG::setSeed(int seed) {
    gen = std::mt19937(seed);
}

int RNG::getSeed() {
    return gen();
}

std::string RNG::randomCreature(double lvl) {
    int length = 0; 
    if (lvl == 1.0) {
        length = sizeof(basic_lvl1) / sizeof(basic_lvl1[0]);
        return basic_lvl1[nextInt(0, length - 1)];
    } else if (lvl == 1.5) {
        length = sizeof(upgraded_lvl1) / sizeof(upgraded_lvl1[0]);
        return upgraded_lvl1[nextInt(0, length - 1)];
    } else if (lvl == 2.0) {
        length = sizeof(basic_lvl2) / sizeof(basic_lvl2[0]);
        return basic_lvl2[nextInt(0, length - 1)];
    } else if (lvl == 2.5) {
        length = sizeof(upgraded_lvl2) / sizeof(upgraded_lvl2[0]);
        return upgraded_lvl2[nextInt(0, length - 1)];
    } else if (lvl == 3.0) {
        length = sizeof(basic_lvl3) / sizeof(basic_lvl3[0]);
        return basic_lvl3[nextInt(0, length - 1)];
    } else if (lvl == 3.5) {
        length = sizeof(upgraded_lvl3) / sizeof(upgraded_lvl3[0]);
        return upgraded_lvl3[nextInt(0, length - 1)];
    } else if (lvl == 4.0) {
        length = sizeof(basic_lvl4) / sizeof(basic_lvl4[0]);
        return basic_lvl4[nextInt(0, length - 1)];
    } else if (lvl == 4.5) {
        length = sizeof(upgraded_lvl4) / sizeof(upgraded_lvl4[0]);
        return upgraded_lvl4[nextInt(0, length - 1)];
    } else if (lvl == 5.0) {
        length = sizeof(basic_lvl5) / sizeof(basic_lvl5[0]);
        return basic_lvl5[nextInt(0, length - 1)];
    } else if (lvl == 5.5) {
        length = sizeof(upgraded_lvl5) / sizeof(upgraded_lvl5[0]);
        return upgraded_lvl5[nextInt(0, length - 1)];
    } else if (lvl == 6.0) {
        length = sizeof(basic_lvl6) / sizeof(basic_lvl6[0]);
        return basic_lvl6[nextInt(0, length - 1)];
    } else if (lvl == 6.5) {
        length = sizeof(upgraded_lvl6) / sizeof(upgraded_lvl6[0]);
        return upgraded_lvl6[nextInt(0, length - 1)];
    } else if (lvl == 7.0) {
        length = sizeof(basic_lvl7) / sizeof(basic_lvl7[0]);
        return basic_lvl7[nextInt(0, length - 1)];
    } else if (lvl == 7.5) {
        length = sizeof(upgraded_lvl7) / sizeof(upgraded_lvl7[0]);
        return upgraded_lvl7[nextInt(0, length - 1)];
    } else {
        return "INVALID LEVEL";
    }
}


