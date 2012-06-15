#pragma once

#import <random>

struct vec {
    vec() {this->x = 0; this->y = 0;};
    vec(float x, float y) {this->x = x; this->y = y;};
    float x, y;
    vec operator+(vec v) {return vec(x + v.x, y + v.y);};
    vec operator-(vec v) {return vec(x - v.x, y - v.y);};
    float dot(vec v) {return x * v.x + y * v.y;}
};

class Perlin {
public:
    Perlin(uint width, uint height, int seed);

    void generate() {};

    float at(float x, float y) const;
private:
    std::mt19937 _rng;
    uint _width;
    uint _height;
    int _seed;

    vec _g(vec v) const;
};
