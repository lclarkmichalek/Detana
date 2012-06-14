#pragma once

#include <iostream>

#include "libtcod/libtcod.hpp"

typedef enum { sea, beach, lowland, highland } Biomes;

class MapTile {
public:
    void calculateBiomes();
    void calculateColor();
    TCODColor getColor() const;

    float& getHeight() {return _height;};
    Biomes& getBiomes() {return _biomes;};
private:
    float _height;
    bool _colored = false;
    TCODColor _color;
    Biomes _biomes;

    void _fail(std::string message, char code = 1) const;
};
