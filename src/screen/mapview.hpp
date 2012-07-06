#pragma once

#include <memory>

#include "libtcod/libtcod.hpp"

#include "./widget.hpp"
#include "./tileinfo.hpp"
#include "../mapgenerator.hpp"

class MapViewer : public Widget {
public:
    MapViewer(std::shared_ptr<MapGenerator> gen);

    void draw(TCODConsole* c, uint off_x, uint off_y);
    void setSize(uint x, uint y);
    void moveCursor(int delta_x, int delta_y);

    void linkTileInfo(std::shared_ptr<TileInfo> info) {_info = info;};
protected:
    std::shared_ptr<MapGenerator> _gen;
    std::shared_ptr<TileInfo> _info;
    uint _width, _height;
    uint _cur_x, _cur_y;
};
