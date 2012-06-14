#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>

#include "diamondsquare.hpp"

DiamondSquare::DiamondSquare(uint width, uint height,
                             int seed, float roughness)
    : _width(width), _height(height), _rough(roughness) {
    if (_rough < 0 || _rough > 1)
        _fail("Rough must be between 0 and 1");

    _iterations = int(ceil(log((std::max(width, height) - 1) / log(2)))) + 2;
    _hm_size = pow(2, _iterations) + 1;
    _hm = new float*[_hm_size];
    for(uint i = 0; i < _hm_size; i++)
        _hm[i] = new float[_hm_size];

    _rng.seed(seed);
}

DiamondSquare::~DiamondSquare() {
    for (uint i = 0; i != _hm_size; i++)
        delete[] _hm[i];
    delete[] _hm;
}

void DiamondSquare::generate() {
    _seed_corners();

    float variation = _rough;
    for(short side_length = _hm_size - 1;
        side_length >= 2;
        side_length /= 2, variation *= _rough) {
        int half_side = side_length / 2;

        for(uint x = 0; x < _hm_size-1; x += side_length) {
            for (uint y = 0; y < _hm_size-1; y += side_length) {
                _square(x, y, side_length, variation);
            }
        }

        for(uint x = 0; x < _hm_size-1; x += half_side) {
            for (uint y = (x + half_side) % side_length;
                 y < _hm_size-1; y += side_length) {
                _diamond(x, y, side_length, variation);
            }
        }
    }

    _normalise_heights();

    _generated = true;
}

float DiamondSquare::at(uint x, uint y) const {
    if (!_generated)
        _fail("Generate has not been run");
    else if (x > _width)
        _fail("X out of bounds");
    else if (y > _height)
        _fail("Y out of bounds");

    return _hm[x][y];
}

void DiamondSquare::_fail(std::string msg, char code) const {
    std::cerr << "DiamondSquare: " << msg << std::endl;
    exit(code);
}

void DiamondSquare::_seed_corners() {
    _hm[0][0] = _random();
    _hm[0][_hm_size-1] = _random();
    _hm[_hm_size-1][0] = _random();
    _hm[_hm_size-1][_hm_size-1] = _random();
}

void DiamondSquare::_square(int x, int y, int side_length, float variation) {
    int half_side = side_length / 2;
    float avg = (_hm[x][y] +
                 _hm[x][(y + side_length)%_hm_size] +
                 _hm[(x + side_length)%_hm_size][y] +
                 _hm[(x + side_length)%_hm_size][(y + side_length)%_hm_size]
                 ) / 4;
    _hm[(x + half_side)%_hm_size][(y + half_side)%_hm_size] = avg +
        _random() * variation - variation/2;
}

void DiamondSquare::_diamond(int x, int y, int side_length, float variation) {
    int half_side = side_length/2;
    float avg = (_hm[(x - half_side)%_hm_size][y] +
                 _hm[(x + half_side)%_hm_size][y] +
                 _hm[x][(y - half_side)%_hm_size] +
                 _hm[x][(y + half_side)%_hm_size]) / 4;
    _hm[x][y] = avg +
        _random() * variation - variation/2;
}

void DiamondSquare::_normalise_heights() {
    float max = -100;
    float min = 100;
    for(uint x = 0; x < _hm_size; x++) {
        for (uint y = 0; y < _hm_size; y ++) {
            float val = _hm[x][y];
            if (val > max)
                max = val;
            if (val < min)
                min = val;
        }
    }
    for(uint x = 0; x < _hm_size; x++) {
        for (uint y = 0; y < _hm_size; y ++) {
            _hm[x][y] = (_hm[x][y] - min) / (max - min);
        }
    }
}

float DiamondSquare::_random() {
    return float(_rng() - _rng.min())/_rng.max();
}
