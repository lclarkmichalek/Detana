#include <iostream>
#include <time.h>

#include "libtcod/libtcod.hpp"

#include "mapgenerator.hpp"
#include "maptile.hpp"

int main() {
    TCODConsole::initRoot(130, 130, "Detana", false);
    TCODSystem::setFps(25);
    TCODConsole* root = TCODConsole::root;
    root->flush();
    int seed = 0;
    seed = (int)time(NULL);
    MapGenerator mapgen(7, seed, 0.7);
    uint length = mapgen.getSize();
    mapgen.genHeightMap();
    mapgen.colorTiles();
    //    mapgen.setHeightRegions();
    //    mapgen.applyHumidity();
    MapGenerator::map_t mp = mapgen.getMap();
    for(uint x = 0; x != length; x++) {
        for(uint y = 0; y != length; y++) {
            MapTile& tile = mapgen.at(x, y);
            root->setCharBackground(x, y, tile.getColor());
        }
    }
    root->flush();

    bool gameRunning = true;
    while(gameRunning && !TCODConsole::isWindowClosed()) {
        TCOD_key_t pressed = TCODConsole::checkForKeypress();
        if (pressed.vk == TCODK_ESCAPE) gameRunning = false;
        };

    return 0;
}
