#pragma once

#include "libtcod/libtcod.hpp"

typedef enum { sea, beach, lowland, highland } Biomes;

class MapTile {
public:
    float& getHeight() {return _height;};
    Biomes& getBiomes() {return _biomes;};

    TCODColor getColor() {return _color;};
    void setColor(TCODColor color) {_color=color;};
private:
    float _height;
    TCODColor _color;
    Biomes _biomes;
};
