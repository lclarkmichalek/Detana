#pragma once

#include <string>
#include <random>

class DiamondSquare {
public:
    DiamondSquare(uint width, uint height, int seed, float roughness = 0.7);
    ~DiamondSquare();

    void generate();

    float at(uint x, uint y) const;
private:
    float** _hm;
    uint _width;
    uint _height;
    uint _hm_size;
    uint _iterations;
    float _rough;

    std::mt19937 _rng;
    float _random();

    bool _generated = false;

    void _fail(std::string message, char code = 1) const;

    void _seed_corners();
    void _square(int x, int y, int side_length, float variation);
    void _diamond(int x, int y, int side_length, float variation);
    void _normalise_heights();
};
