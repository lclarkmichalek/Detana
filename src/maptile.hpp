#pragma once

#include <iostream>

#include "libtcod/libtcod.hpp"

enum Biomes { Sea,
               AridDesert,
               XericShrubland,
               DrySteppe,
               TemperateSteppe,
               Taiga,
               Tundra,
               SemiaridDesert,
               GrassSavanna,
               TemperateForest,
               AlpineTundra,
               TropicalRainforest,
               SubtropicalRainforest,
               MonsoonForest,
               England
};

class MapTile {
public:
    void calculateBiomes();
    void calculateColor();
    void draw(TCODConsole* console, int x, int y) const;

    void setProperties(float height, float temp, float humidity);

    float height;
    float temp;
    float humidity;
    TCODColor color;
    Biomes biomes;

private:
    bool _colored = false;
    void _fail(std::string message, char code = 1) const;
};
