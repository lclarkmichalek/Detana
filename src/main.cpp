#include <iostream>
#include <time.h>
#include <memory>

#include "libtcod/libtcod.hpp"

#include "screen/widget.hpp"
#include "screen/tileinfo.hpp"
#include "screen/mapview.hpp"
#include "screen/linear.hpp"
#include "mapgenerator.hpp"
#include "maptile.hpp"

using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;

int main() {
    TCODConsole::initRoot(100, 100, "Detana", false);
    TCODSystem::setFps(60);
    TCODConsole* root = TCODConsole::root;

    int seed = 0;
    seed = (int)time(NULL);
    auto mapgen = make_shared<MapGenerator>(7, seed, 0.7);
    mapgen->genHeightMap();
    mapgen->colorTiles();

    auto layout = make_shared<HorizontalLayout>(TCODColor::blue);
    shared_ptr<TileInfo> tileinfo = make_shared<TileInfo>();
    shared_ptr<MapViewer> mapview = make_shared<MapViewer>(mapgen);
    tileinfo->setColors(TCODColor::black, TCODColor::white);
    mapview->linkTileInfo(tileinfo);
    layout->addWidget(dynamic_pointer_cast<Widget>(tileinfo));
    layout->addWidget(dynamic_pointer_cast<Widget>(mapview));
    layout->setSizeBias(1, 3);

    shared_ptr<Widget> root_widget = dynamic_pointer_cast<Widget>(layout);
    root_widget->setSize(100, 100);

    root->setKeyboardRepeat(250, 100);

    bool gameRunning = true;
    while(gameRunning && !TCODConsole::isWindowClosed()) {
        TCOD_key_t pressed = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
        if (pressed.vk == TCODK_ESCAPE) gameRunning = false;
        int delta_x = 0;
        int delta_y = 0;
        switch(pressed.vk) {
        case TCODK_LEFT:
            delta_x--;
            break;
        case TCODK_RIGHT:
            delta_x++;
            break;
        case TCODK_UP:
            delta_y--;
            break;
        case TCODK_DOWN:
            delta_y++;
            break;
        default:
            break;
            // Ignore
        }
        mapview->moveCursor(delta_x, delta_y);

        root_widget->draw(root, 0, 0);
        root->flush();
    };

    return 0;
}
