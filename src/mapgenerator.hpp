#pragma once

#include <random>
#include <vector>
#include <utility>

#include "maptile.hpp"
#include "diamondsquare.hpp"

typedef std::pair<int, int> coord;

class MapGenerator {
public:
    MapGenerator(char size_n,
                 int seed = -1,
                 double rough = 0.5);

    // Runs the generator for one pass, or until timeout expires.
    bool runPass(uint timeout);

    // Runs the generator until timeout expires.
    bool genHeightMap();
    void colorTiles();

    bool setHeightRegions();

    bool applyHumidity();

    typedef std::vector<std::vector<MapTile>> map_t;
    MapTile& at(int x, int y);
    MapTile& at(coord c);
    const map_t& getMap();
    ushort getSize();
private:

    const ushort _size;
    const int _seed;
    const double _rough;
    std::mt19937 _rng;
    // Returns a rand number [0, 1]
    float random();

    map_t _hm;
    // The coordinates of the map in height order
    std::vector<coord> _height_order;
    short _pass_number;

    DiamondSquare _height_map;

    //    void generateSortedHeights();
    // Returns the surrounding points
    std::vector<coord> get_neighbours(coord);

    inline float& gh(int x, int y) {
        return _hm.at(x%_hm.size()).at(y%_hm.at(0).size()).getHeight();
    };
};
