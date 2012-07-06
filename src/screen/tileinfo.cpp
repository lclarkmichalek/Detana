#include "./tileinfo.hpp"

#include <sstream>

void TileInfo::setTile(std::shared_ptr<MapTile> tile, uint x, uint y) {
    clearText();
    setAlignments(Alignment::Top, Alignment::Left);

    std::stringstream line1, line2, line3, line4, line5;
    line1 << "Map tile at (" << x << ", " << y << ")";
    addLine(line1.str());

    // Blank line
    addLine("");

    line2 << "Temprature: " << tile->temp;
    addLine(line2.str());

    line3 << "Height: " << tile->height;
    addLine(line3.str());

    line4 << "Humidity: " << tile->humidity;
    addLine(line4.str());
}
