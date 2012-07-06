#pragma once

#include <memory>

#include "libtcod/libtcod.hpp"

#include "./widget.hpp"

class Layout : public Widget {
public:
    virtual unsigned char addWidget(std::shared_ptr<Widget> w) = 0;
    virtual std::shared_ptr<Widget> getWidget(unsigned char p) = 0;
    virtual void clearWidgets() = 0;

    virtual void setSizeBias(unsigned char p, float bias) = 0;
    virtual void setBorder(bool e, TCODColor c=TCODColor::black) = 0;
};
