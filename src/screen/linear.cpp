#include "linear.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "libtcod/libtcod.hpp"

#include "./widget.hpp"
#include "./layout.hpp"

LinearLayout::LinearLayout()
    : _border(false),
      _width(0),
      _height(0) {
    return;
}

LinearLayout::LinearLayout(TCODColor border_color)
    : _border(true),
      _border_color(border_color),
      _width(0), _height(0) {
    return;
}

unsigned char LinearLayout::addWidget(std::shared_ptr<Widget> w) {
    int index = _widgets.size();
    _widgets.push_back(w);
    _biases.push_back(1);
    return index;
}

std::shared_ptr<Widget> LinearLayout::getWidget(unsigned char index) {
    return _widgets.at(index); // .at throws
}

void LinearLayout::clearWidgets() {
    _widgets.clear();
    _biases.clear();
}

void LinearLayout::setSizeBias(unsigned char p, float bias) {
    if (p >= _biases.size())
        return;
    _biases[p] = bias;
}

void LinearLayout::setSize(uint x, uint y) {
    _width = x;
    _height = y;
}

void LinearLayout::setBorder(bool e, TCODColor c) {
    _border = e;
    if (_border)
        _border_color = c;
}

void hline(TCODConsole* c, uint x, uint y, uint len, TCODColor color) {
    c->setDefaultBackground(color);
    c->setDefaultForeground(color);
    c->hline(x, y, len, TCOD_BKGND_SET);
}

void vline(TCODConsole* c, uint x, uint y, uint len, TCODColor color) {
    c->setDefaultBackground(color);
    c->setDefaultForeground(color);
    c->vline(x, y, len, TCOD_BKGND_SET);
}

void HorizontalLayout::draw(TCODConsole* c, uint off_x, uint off_y) {
    uint awidth = _width;
    uint aheight = _height;

    if (_border) {
        hline(c, off_x, off_y, _width - 1, _border_color);
        hline(c, off_x, off_y + _height - 1, _width - 1, _border_color);
        vline(c, off_x, off_y, _height, _border_color);

        awidth -= _biases.size() + 1;
        aheight -= 2;
        off_x++;
        off_y++;
    }

    float total_biases = 0;
    for(auto i = _biases.begin(); i != _biases.end(); i++)
        total_biases += *i;
    float per_bias = awidth / total_biases;

    int x = off_x;
    auto b = _biases.begin();
    auto w = _widgets.begin();
    while (b != _biases.end()) {

        int width = per_bias * (*b);
        (*w)->setSize(width, aheight);
        (*w)->draw(c, x, off_y);
        if (_border) {
            vline(c, x+width, off_y, aheight, _border_color);
            x += width + 1;
        } else
            x += width;
        b++; w++;
    }
}

void VerticalLayout::draw(TCODConsole* c, uint off_x, uint off_y) {
    uint awidth = _width;
    uint aheight = _height;

    if (_border) {
        vline(c, off_x, off_y, _height - 1, _border_color);
        vline(c, off_x + _width - 1, off_y, _height - 1, _border_color);
        hline(c, off_x, off_y, _width, _border_color);

        aheight -= _biases.size() + 1;
        awidth -= 2;
        off_x++;
        off_y++;
    }

    float total_biases = 0;
    for(auto i = _biases.begin(); i != _biases.end(); i++)
        total_biases += *i;
    float per_bias = aheight / total_biases;

    int y = off_y;
    auto b = _biases.begin();
    auto w = _widgets.begin();
    while (b != _biases.end()) {
        int height = per_bias * (*b);
        (*w)->setSize(awidth, height);
        (*w)->draw(c, off_x, y);
        if (_border) {
            hline(c, off_x, y + height, aheight + 1, _border_color);
            y += height + 1;
        } else
            y += height;
        b++; w++;
    }
}
