#pragma once

#include <random>
#include <algorithm> 

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
    double nextDoubleRounded(double min, double max);
    bool nextBool();
    bool nextBool(float probability);
    void setSeed(int seed);
    int getSeed();
    std::string randomCreature(double lvl);

    template<typename T>
    void shuffle(std::vector<T>& vec);


private:
    std::random_device rd;
    std::mt19937 gen;
};

template<typename T>
void RNG::shuffle(std::vector<T>& vec) {
    std::shuffle(vec.begin(), vec.end(), gen);
}