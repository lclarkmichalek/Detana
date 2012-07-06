#pragma once

#include <string>
#include <vector>

#include "libtcod/libtcod.hpp"

#include "./widget.hpp"

enum Alignment {
    Top,
    Bottom,
    Center,
    Left,
    Right
};

class TextWidget : public Widget {
public:
    TextWidget() {};
    TextWidget(std::vector<std::string> lines, TCODColor text, TCODColor back);

    void draw(TCODConsole* c, uint x=0, uint y=0);

    void setSize(uint x, uint y);

    unsigned char addLine(std::string line);
    void setText(std::vector<std::string> lines);
    void clearText();

    void setAlignments(Alignment val, Alignment hal);
    void setColors(TCODColor text, TCODColor back);
protected:
    std::vector<std::string> _lines;
    Alignment _val = Alignment::Center;
    Alignment _hal = Alignment::Center;
    uint _width, _height;
    TCODColor _text_color, _background_color;
};
