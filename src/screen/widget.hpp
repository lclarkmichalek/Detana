#pragma once

#include "libtcod/libtcod.hpp"

class Widget {
public:
    virtual void draw(TCODConsole* c, uint x=0, uint y=0) = 0;

    virtual void setSize(uint x, uint y) = 0;
};
