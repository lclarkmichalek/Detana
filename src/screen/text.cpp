#include "text.hpp"

#include <string>
#include <iostream>
#include <vector>

#include "libtcod/libtcod.hpp"

// TODO: Handle where text > size available
// TODO: Allow spacing between lines

TextWidget::TextWidget(std::vector<std::string> lines,
                       TCODColor text, TCODColor back)
    : _lines(lines),
      _text_color(text),
      _background_color(back){};

void TextWidget::setSize(uint x, uint y) {
    _width = x;
    _height = y;
}

unsigned char TextWidget::addLine(std::string line) {
    unsigned char pos = _lines.size();
    _lines.push_back(line);
    return pos;
}

void TextWidget::setText(std::vector<std::string> lines) {
    _lines = lines;
}

void TextWidget::clearText() {
    _lines.clear();
}

void TextWidget::setAlignments(Alignment val, Alignment hal) {
    _val = val;
    _hal = hal;
}

void TextWidget::setColors(TCODColor text, TCODColor background) {
    _text_color = text;
    _background_color = background;
}

void TextWidget::draw(TCODConsole* c, uint off_x, uint off_y) {
    c->setAlignment(TCOD_LEFT);

    c->setDefaultBackground(_background_color);
    c->setDefaultForeground(_text_color);
    c->rect(off_x, off_y, _width, _height, true, TCOD_BKGND_SET);

    int text_start;
    switch (_val) {
    case Alignment::Top:
        text_start = off_y;
        break;
    case Alignment::Bottom:
        text_start = off_y + _height - _lines.size();
        break;
    default:
        std::cerr << "Text widget given invalid vertical" <<
            " alignment, defaulting to Center" << std::endl;
        // Drop through to Center
    case Alignment::Center:
        text_start = off_y + (_height - _lines.size())/2;
        break;
    }

    int y = text_start;
    for (auto t = _lines.begin(); t != _lines.end(); t++) {
        int x;
        switch(_hal) {
        case Alignment::Left:
            x = off_x;
            break;
        case Alignment::Right:
            x = off_x + _width - t->size();
        default:
            std::cerr << "Text widget given invalid horizontal" <<
            " alignment, defaulting to Center" << std::endl;
        // Drop through to Center
        case Alignment::Center:
            x = off_x + (_width - t->size())/2;
            break;
        }
        c->print(x, y, t->c_str());
        y += 1;
    }
}
