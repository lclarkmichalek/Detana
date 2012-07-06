#pragma once

#include <memory>

#include "libtcod/libtcod.hpp"

#include "./text.hpp"
#include "../maptile.hpp"

class TileInfo : public TextWidget {
public:
    void setTile(std::shared_ptr<MapTile> tile, uint x, uint y);
protected:
    std::shared_ptr<MapTile> _tile;
};
