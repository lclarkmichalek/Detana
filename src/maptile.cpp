#include "maptile.hpp"

#include <iostream>
#include <random>

float water_level = 0.25;
float mount_level = 0.6;

void MapTile::draw(TCODConsole* console, int x, int y) const {
    console->setCharBackground(x, y, _color);
}

void MapTile::calculateBiomes() {
    if (_height < water_level)
        _biomes = Biomes::sea;
    else if (_height < mount_level)
        _biomes = Biomes::lowland;
    else
        _biomes = Biomes::highland;
}

void MapTile::calculateColor() {
    switch (_biomes) {
    case Biomes::sea: {
        TCODColor from;
        from = TCODColor::darkAzure;

        float lerp_factor = 1 - _height/water_level;
        _color = TCODColor::lerp(from,
                                 TCODColor::black,
                                 lerp_factor);
        break;
    }

    case Biomes::lowland: {
        float lerp_factor = (_height - water_level) /
            (mount_level - water_level);
        TCODColor from;
        from = TCODColor::darkerGreen;
        _color = TCODColor::lerp(from,
                                 TCODColor::darkerGrey,
                                 lerp_factor);
        break;
    }

    case Biomes::highland: {
        TCODColor from;
        float lerp_factor = (_height - mount_level)/(1 - mount_level);
        _color = TCODColor::lerp(TCODColor::darkerGrey,
                                 TCODColor::white,
                                 lerp_factor);
        break;
    }

    default:
        _fail("Colouring tile without a biomes");
    }
    _colored = true;
}

TCODColor MapTile::getColor() const {
    if (!_colored)
        _fail("Taking color of uncoloured tile.");
    return _color;
}

void MapTile::_fail(std::string msg, char code) const {
    std::cerr << "MapTile: " << msg << std::endl;
    exit(code);
}
