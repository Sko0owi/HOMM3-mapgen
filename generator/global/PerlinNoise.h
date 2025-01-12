
#include <vector>


class RNG;

class PerlinNoise {
public:
    // Constructor with optional seed
    PerlinNoise(RNG *rng);
    
    // Generate noise at a single point
    double noise(double x, double y);
    
    // Generate a grid of noise values
    std::vector<std::vector<double>> generateNoiseGrid(
        int width,
        int height,
        double scale = 1.0
    );
private:
    std::vector<int> p;
    RNG *rng;
    
    // Private helper functions
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y);
};