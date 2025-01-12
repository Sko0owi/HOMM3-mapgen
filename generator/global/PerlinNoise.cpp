#include <bits/stdc++.h>
#include "./PerlinNoise.h"
#include "./Random.h"

double PerlinNoise::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y) {
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

PerlinNoise::PerlinNoise(RNG *rng) : rng(rng) {
    // Initialize the permutation vector
    p.resize(256);
    for(int i = 0; i < 256; i++) {
        p[i] = i;
    }
    rng->shuffle(p);
    p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y) {
    // Find unit grid cell containing point
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    
    // Find relative x, y of point in cell
    x -= std::floor(x);
    y -= std::floor(y);
    
    // Compute fade curves
    double u = fade(x);
    double v = fade(y);
    
    // Hash coordinates of the 4 square corners
    int A = p[X] + Y;
    int AA = p[A];
    int AB = p[A + 1];
    int B = p[X + 1] + Y;
    int BA = p[B];
    int BB = p[B + 1];
    
    // Add blended results from 4 corners of square
    return lerp(v, 
        lerp(u, grad(p[AA], x, y),
                grad(p[BA], x - 1, y)),
        lerp(u, grad(p[AB], x, y - 1),
                grad(p[BB], x - 1, y - 1)));
}

std::vector<std::vector<double>> PerlinNoise::generateNoiseGrid(
    int width,
    int height,
    double scale
) {
    std::vector<std::vector<double>> noiseGrid(height, std::vector<double>(width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sampleX = x * scale;
            double sampleY = y * scale;
            noiseGrid[y][x] = noise(sampleX, sampleY);
            noiseGrid[y][x] = (noiseGrid[y][x] > 0.0 ? 1 : 0);
        }
    }
    
    return noiseGrid;
}