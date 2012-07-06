#pragma once

#include <vector>
#include <memory>

#include "libtcod/libtcod.hpp"

#include "./widget.hpp"
#include "./layout.hpp"

class LinearLayout : public Layout {
public:
    LinearLayout();
    LinearLayout(TCODColor border_color);

    unsigned char addWidget(std::shared_ptr<Widget> w);
    std::shared_ptr<Widget> getWidget(unsigned char p);
    void clearWidgets();

    void setSizeBias(unsigned char p, float bias);
    void setSize(uint x, uint y);
    void setBorder(bool e, TCODColor c);
protected:
    std::vector<std::shared_ptr<Widget>> _widgets;
    std::vector<float> _biases;

    bool _border;
    TCODColor _border_color = TCODColor::green;

    uint _width, _height;
};

class HorizontalLayout : public LinearLayout {
public:
    HorizontalLayout(TCODColor color) : LinearLayout(color) {};
    void draw(TCODConsole* c, uint x=0, uint y=0);
};

class VerticalLayout : public LinearLayout {
public:
    VerticalLayout(TCODColor color) : LinearLayout(color) {};
    void draw(TCODConsole* c, uint x=0, uint y=0);
};
