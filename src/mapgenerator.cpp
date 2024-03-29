#include <vector>
#include <functional>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <utility>
#include <algorithm>

#include "mapgenerator.hpp"

noise::module::Curve* curve_modifier(noise::module::Module* mod,
                                     double f(double)) {
    noise::module::Curve* curved = new noise::module::Curve;
    for (double x = -1; x < 1.0; x += 0.9) {
        double y = f(x);
        curved->AddControlPoint(x, y);
    }
    curved->SetSourceModule(0, *mod);
    return curved;
}

noise::module::Clamp* clamp_modifier(noise::module::Module* mod) {
    noise::module::Clamp* clamped = new noise::module::Clamp;
    clamped->SetBounds(-1, 1);
    clamped->SetSourceModule(0, *mod);
    return clamped;
}

MapGenerator::MapGenerator(char size_n, int seed, double rough)
    : _size(pow(2, size_n)),
      _seed(seed),
      _rough(rough),
      _height_map(_size, _size, seed, rough) {

    _hm.reserve(sizeof(std::vector<int>) * _size);
    for(int i = 0; i < _size; i++) {
        std::vector<MapTile> row;
        row.reserve(sizeof(int)*_size);
        for(int j = 0; j < _size; j++)
            row.push_back(MapTile());
        _hm.push_back(row);
    }

    std::cout << "Generating map of dimensions " <<
        _hm.size() << "x" << _hm.at(0).size() << std::endl;

    _rng.seed(seed);

    noise::module::Perlin* rain_map = new noise::module::Perlin;
    rain_map->SetSeed(seed);
    rain_map->SetFrequency(0.05);
    _humidity_map =
        noise::model::Plane(*curve_modifier(clamp_modifier(rain_map),
                             [](double x){return (pow(x, 3) + x + 2)/4;}));

    noise::module::Perlin* temp_map = new noise::module::Perlin;
    temp_map->SetSeed(seed *123653);
    temp_map->SetFrequency(0.01);
    _temprature_map =
        noise::model::Plane(*curve_modifier(clamp_modifier(temp_map),
                             [](double x) {return (pow(x, 3) + x + 2)/4;}));
}

bool MapGenerator::genHeightMap() {
    _height_map.generate();
    for (int x = 0; x != _size; x++) {
        for (int y = 0; y != _size; y++) {
            at(x, y).setProperties(_height_map.at(x, y),
                                   _temprature_map.GetValue(x, y),
                                   _humidity_map.GetValue(x, y));
        }
    }
    //    generateSortedHeights();

    return true;
}

void MapGenerator::colorTiles() {
    for (int x = 0; x != _size; x++) {
        for (int y = 0; y != _size; y++) {
            MapTile& tile = at(x, y);
            tile.calculateBiomes();
            tile.calculateColor();
        }
    }
}
/*
void MapGenerator::generateSortedHeights() {
    typedef std::map<std::pair<int, int>, float> hm_t;
    typedef std::pair<int, int> coord;
    hm_t height_map;
    for(int x = 0; x < _size; x++) {
        for (int y = 0; y < _size; y ++) {
            float& val = gh(x, y);
            height_map[std::make_pair(x, y)] = val;
        }
    }
    std::vector<std::pair<coord, float> >
        sorted(height_map.begin(), height_map.end());
    std::sort(sorted.begin(), sorted.end(), IntCmp<coord, float>());
    _height_order.clear();
    _height_order.reserve(sorted.size());
    for(auto it = sorted.begin(); it != sorted.end(); it++) {
        _height_order.push_back(it->first);
    }
}
*/
std::vector<coord> MapGenerator::get_neighbours(coord c) {
    std::vector<coord> coords;
    if (c.second != 0)
        coords.push_back(std::make_pair(c.first, c.second - 1));
    if (c.second != _size - 1)
        coords.push_back(std::make_pair(c.first, c.second + 1));
    if (c.first != 0)
        coords.push_back(std::make_pair(c.first - 1, c.second));
    if (c.first != _size - 1)
        coords.push_back(std::make_pair(c.first + 1, c.second));
    return coords;
}

bool MapGenerator::applyHumidity() {
    return true;
    /*    std::cout << "Starting humid" << std::endl;
    for(int x = 0; x < _size; x++) {
        for (int y = 0; y < _size; y ++) {
            MapTile& tile = at(x, y);
            if (tile.getBiomes() == Biomes::sea)
                continue;
            float humid = perlin_at(x, y);

            TCODColor color = TCODColor::lerp(tile.getColor(),
                                              TCODColor::yellow,
                                              humid);
            tile.setColor(color);
        }
    }
    std::cout << "End humid" << std::endl;
    return true;*/
}

MapTile& MapGenerator::at(int x, int y) {
    return _hm.at(x).at(y);
}

MapTile& MapGenerator::at(coord c) {
    return _hm.at(c.first).at(c.second);
}

const MapGenerator::map_t& MapGenerator::getMap() {
    return _hm;
}

ushort MapGenerator::getSize() {
    return _size;
}

float MapGenerator::random() {
    return float(_rng() - _rng.min()) / _rng.max();
}
