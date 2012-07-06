#include "./mapview.hpp"

#include <memory>

#include "libtcod/libtcod.hpp"

#include "../mapgenerator.hpp"

MapViewer::MapViewer(std::shared_ptr<MapGenerator> gen)
    : _gen(gen),
      _cur_x(10),
      _cur_y(10) {}

void MapViewer::setSize(uint x, uint y) {
    _width = x;
    _height = y;
}

void MapViewer::moveCursor(int delta_x, int delta_y) {
    if (delta_x < 0 && _cur_x < uint(-delta_x))
        _cur_x = 0;
    else
        _cur_x += delta_x;

    if (delta_y < 0 && _cur_y < uint(-delta_y))
        _cur_y = 0;
    else
        _cur_y += delta_y;

    if (_info.get() != 0)
        _info->setTile(std::make_shared<MapTile>(_gen->at(_cur_x, _cur_y)),
                       _cur_x, _cur_y);
}

void MapViewer::draw(TCODConsole* c, uint off_x, uint off_y) {
    for (uint x = 0; x != _gen->getSize() && x != _width; x++)
        for (uint y = 0; y != _gen->getSize() && y != _height; y++) {
            MapTile& tile = _gen->at(x, y);
            c->putChar(off_x + x, off_y + y, ' ');
            tile.draw(c, off_x + x, off_y + y);
        }

    if (_cur_x < _width && _cur_x < _gen->getSize() &&
        _cur_y < _height && _cur_y < _gen->getSize())
        c->putChar(off_x + _cur_x, off_y + _cur_y, 'X');
}
