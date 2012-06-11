#include <vector>
#include <functional>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <utility>
#include <algorithm>

#include "mapgenerator.hpp"

MapGenerator::MapGenerator(char size_n, int seed, double rough)
    : _size(pow(2, size_n) + 1),
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

    //    _height_map = DiamondSquare(_size, _size, seed, rough);

    _rng.seed(seed);
}

bool MapGenerator::genHeightMap() {
    _height_map.generate();
    //    generateSortedHeights();

    return true;
}

void MapGenerator::colorTiles() {
    for (int x = 0; x != _size; x++) {
        for (int y = 0; y != _size; y++) {
            MapTile& tile = at(x, y);
            tile.setColor(TCODColor::lerp(TCODColor::white,
                                          TCODColor::black,
                                          _height_map.at(x, y)));
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

bool MapGenerator::setHeightRegions() {
    float water_proportion = 0.25;
    float earth_proportion = 0.75;
    MapTile top_water = at(_height_order[water_proportion *
                                         _height_order.size()]);
    MapTile top_earth = at(_height_order[earth_proportion *
                                         _height_order.size()]);
    MapTile top = at(_height_order[_height_order.size() - 1]);
    std::cout << "Water height: " << top_water.getHeight() << std::endl;
    std::cout << "Earth height: " << top_earth.getHeight() << std::endl;
    std::cout << "Top: " << top.getHeight() << std::endl;
    for (auto coord = _height_order.begin();
         coord != _height_order.end(); coord++) {
        MapTile& tile = at(*coord);
        if (tile.getHeight() < top_water.getHeight()) {
            tile.getBiomes() = Biomes::sea;

            TCODColor from;
            if (random() < 0.95)
                from = TCODColor::darkAzure;
            else
                from = TCODColor::darkSky;
            float lerp_factor = 1 - tile.getHeight()/top_water.getHeight();
            TCODColor color = TCODColor::lerp(from,
                                              TCODColor::black,
                                              lerp_factor);
            tile.setColor(color);
        } else if (tile.getHeight() < top_earth.getHeight()) {
            tile.getBiomes() = Biomes::lowland;

            float lerp_factor = (tile.getHeight() - top_water.getHeight()) /
                (top_earth.getHeight() - top_water.getHeight());
            TCODColor from;
            float rand = random();
            if (rand < 0.2)
                from = TCODColor::darkSepia;
            else
                from = TCODColor::darkerGreen;
            TCODColor color = TCODColor::lerp(from,
                                              TCODColor::darkerGrey,
                                              lerp_factor);
            tile.setColor(color);
        } else {
            tile.getBiomes() = Biomes::highland;

            float lerp_factor = (tile.getHeight() - top_earth.getHeight()) /
                (top.getHeight() - top_earth.getHeight());
            TCODColor color = TCODColor::lerp(TCODColor::darkerGrey,
                                              TCODColor::white,
                                              lerp_factor);
            tile.setColor(color);
        }
    }

    for (auto coord = _height_order.begin();
         coord != _height_order.end(); coord++) {
        if (at(*coord).getBiomes() != Biomes::sea)
            continue;
        auto neighbours = get_neighbours(*coord);
        for(auto neigh = neighbours.begin();
            neigh != neighbours.end(); neigh++) {
            MapTile& tile = at(*neigh);
            if (tile.getBiomes() == Biomes::lowland && random() < 0.2) {
                tile.getBiomes() = Biomes::beach;
                tile.setColor(TCODColor::lightAmber);
            }
        }
    }
    return true;
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
