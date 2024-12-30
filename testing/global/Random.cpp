#include "./Random.h"

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

