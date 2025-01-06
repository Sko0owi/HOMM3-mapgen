#pragma once

#include <random>

class RNG {
public:
    RNG();
    RNG(int seed);
    int nextInt();
    int nextInt(int min, int max);
    float nextFloat();
    float nextFloat(float min, float max);
    double nextDouble();
    double nextDouble(double min, double max);
    bool nextBool();
    bool nextBool(float probability);
    void setSeed(int seed);
    int getSeed();  
private:
    std::random_device rd;
    std::mt19937 gen;
};