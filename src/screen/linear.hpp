#pragma once

#include <vector>
#include <memory>

#include "libtcod/libtcod.hpp"

#include "./widget.hpp"
#include "./layout.hpp"

class LinearLayout : Layout {
public:
    LinearLayout();
    LinearLayout(TCODColor border_color);
    ~LinearLayout();

    uchar addWidget(std::shared_ptr<Widget> w);
    std::shared_ptr<Widget> getWidget(uchar p);
    void clearWidgets();

    void setSizeBias(uchar p, float bias);
    void setSize(uint x, uint y);
    void setBorder(bool e, TCODColor c);
protected:
    std::vector<std::shared_ptr<Widget>> _widgets;
    std::vector<float> _biases;

    bool _border;
    TCODColor _border_color = TCODColor::green;

    uint _width, _height;
};

class HorizontalLayout : LinearLayout {
public:
    void draw(TCODConsole* c, uint x=0, uint y=0);
};

class VerticalLayout : LinearLayout {
public:
    void draw(TCODConsole* c, uint x=0, uint y=0);
};
