#include <iostream>

#include "maptile.hpp"

float water_level = 0.25;
float mount_level = 0.6;

float desert_humidity = 0.15;
float grassland_humidity = 0.7;

float vhot_temp = 0.9;
float hot_temp = 0.75;
float walm_temp = 0.5;
float cold_temp = 0.25;
float vcold_temp = 0.1;

void MapTile::draw(TCODConsole* console, int x, int y) const {
    console->setCharBackground(x, y, color);
}

void MapTile::setProperties(float height, float temp, float humidity) {
    if (height > mount_level)
        temp -= (height - mount_level) / (1 - mount_level);

    this->height = height;
    this->temp = temp;
    this->humidity = humidity;
}

void MapTile::calculateBiomes() {
    if (height < water_level) {
        biomes = Biomes::Sea;
        return;
    }

    if (humidity < desert_humidity) {
        if (temp > vhot_temp)
            biomes = Biomes::AridDesert;
        else if (temp > hot_temp)
            biomes = Biomes::XericShrubland;
        else if (temp > walm_temp)
            biomes = Biomes::DrySteppe;
        else if (temp > cold_temp)
            biomes = Biomes::TemperateSteppe;
        else if (temp > vcold_temp)
            biomes = Biomes::Taiga;
        else
            biomes = Biomes::Tundra;
        return;
    } else if (humidity < grassland_humidity) {
        if (temp > vhot_temp)
            biomes = Biomes::SemiaridDesert;
        else if (temp > hot_temp)
            biomes = Biomes::GrassSavanna;
        else if (temp > walm_temp)
            biomes = Biomes::TemperateForest;
        else if (temp > cold_temp)
            biomes = Biomes::AlpineTundra;
        else if (temp > vcold_temp)
            biomes = Biomes::Taiga;
        else
            biomes = Biomes::Tundra;
        return;
    } else {
        if (temp > vhot_temp)
            biomes = Biomes::TropicalRainforest;
        else if (temp > hot_temp)
            biomes = Biomes::SubtropicalRainforest;
        else if (temp > walm_temp)
            biomes = Biomes::MonsoonForest;
        else if (temp > cold_temp)
            biomes = Biomes::England;
        else if (temp > vcold_temp)
            biomes = Biomes::Taiga;
        else
            biomes = Biomes::Tundra;
        return;
    }
}

void MapTile::calculateColor() {
    TCODColor from = TCODColor::green;
    TCODColor to = TCODColor::green;
    float lerp = 0;

    switch (biomes) {
    case Biomes::Sea: {
        from = TCODColor::darkAzure;
        to = TCODColor::black;
        lerp = 1 - height/water_level;
        break;
    }

    case Biomes::AridDesert: {
        from = TCODColor::darkAmber;
        to = TCODColor::darkerOrange;
        lerp = 1 - (temp - vhot_temp) / (1 - vhot_temp);
        break;
    }

    case Biomes::XericShrubland: {
        from = TCODColor::darkAmber;
        to = TCODColor:: lightYellow;
        lerp = 1 - (temp - hot_temp) / (vhot_temp - hot_temp);
        break;
    }

    case Biomes::DrySteppe: {
        from = TCODColor::lightYellow;
        to = TCODColor::lightChartreuse;
        lerp = 1 - (temp - walm_temp) / (hot_temp - walm_temp);
        break;
    }

    case Biomes::TemperateSteppe: {
        from = TCODColor::lightChartreuse;
        to = TCODColor::darkChartreuse;
        lerp = 1 - (temp - cold_temp) / (walm_temp - cold_temp);
        break;
    }

    case Biomes::Taiga: {
        from = TCODColor::darkChartreuse;
        to = TCODColor::sea;
        lerp = 1 - (temp - vcold_temp) / (cold_temp - vcold_temp);
        break;
    }

    case Biomes::Tundra: {
        from = TCODColor::sea;
        to = TCODColor::lighterSea;
        lerp = 1 - (temp -vcold_temp) / vcold_temp;
    }
        /*
    case Biomes::SemiaridDesert: {
        from = TCODColor::darkAmber;
        to = TCODColor::lightLime;
        lerp = 1 - (temp - vhot_temp) / (1 - vhot_temp);
    }

    case Biomes::GrassSavanna: {
        from = TCODColor::lightYellow;
        to = TCODColor::amber;
        lerp = 1 - (temp - hot_temp) / (vhot_temp - hot_temp);
        break;
    }
        */
    default: {
        //        _fail("Colouring tile without a biomes");
    }
    }
    color = TCODColor::lerp(from, to, lerp);
    _colored = true;
}

void MapTile::_fail(std::string msg, char code) const {
    std::cerr << "MapTile: " << msg << std::endl;
    exit(code);
}
