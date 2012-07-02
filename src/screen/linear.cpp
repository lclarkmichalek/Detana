#include "linear.hpp"

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

uchar LinearLayout::addWidget(std::shared_ptr<Widget> w) {
    int index = _widgets.size();
    _widgets.push_back(w);
    _biases.push_back(1);
    return index;
}

std::shared_ptr<Widget> LinearLayout::getWidget(uchar index) {
    return _widgets.at(index); // .at throws
}

void LinearLayout::clearWidgets() {
    _widgets.clear();
    _biases.clear();
}

void LinearLayout::setSizeBias(uchar p, float bias) {
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

void HorizontalLayout::draw(TCODConsole* c, int off_x, int off_y) {
    uint awidth = _width;
    uint aheight = _height;

    if (_border) {
        c->setDefaultForeground(_border_color);
        c->hline(off_x, off_y, _width);
        c->hline(off_x, off_y + _height, _width);
        c->vline(off_x, off_y, _height);

        awidth -= _biases.size() + 1;
        aheight -= 2;
        off_x++;
        off_y++;
    }

    float total_biases = 0;
    for(auto i = _biases.begin(); i != _biases.end(); i++)
        total_biases += *i;
    float per_bias = awidth / total_bias;

    int x = off_x;
    for (auto b = _biases.begin(), auto w = _widgets.begin();
         b != _biases.end(); b++, w++) {

        int width = per_bias * (*b);
        (*w)->setSize(width, aheight);
        (*w)->draw(c, x, off_y);
        if (_border) {
            c->setDefaultForeground(_border_color);
            c->vline(x+width, off_y, aheight);
            x += width + 1;
        } else
            x += width;
    }
}

void VerticalLayout::draw(TCODConsole* c, int off_x, int off_y) {
    uint awidth = _width;
    uint aheight = _height;

    if (_border) {
        c->setDefaultForeground(_border_color);
        c->vline(off_x, off_y, _height);
        c->vline(off_x + _width, off_y, _height);
        c->hline(off_x, off_y, _width);

        aheight -= _biases.size() + 1;
        awidth -= 2;
        off_x++;
        off_y++;
    }

    float total_biases = 0;
    for(auto i = _biases.begin(); i != _biases.end(); i++)
        total_biases += *i;
    float per_bias = aheight / total_bias;

    int y = off_y;
    for (auto b = _biases.begin(), auto w = _widgets.begin();
         b != _biases.end(); b++, w++) {

        int height = per_bias * (*b);
        (*w)->setSize(awidth, height);
        (*w)->draw(c, off_x, y);
        if (_border) {
            c->setDefaultForeground(_border_color);
            c->hline(off_x, y + height, aheight);
            y += height + 1;
        } else
            y += height;
    }
}
