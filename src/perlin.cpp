#define _USE_MATH_DEFINES
#include <math.h>

#include "perlin.hpp"

Perlin::Perlin(uint width, uint height, int seed) :
    _width(width), _height(height), _seed(seed)
{
    _rng.seed(seed);
}

float Perlin::at(float x, float y) const {
    // point
    vec p = vec(x, y);

    // Bottom left, bottom right, top left, top right
    vec bl, br, tl, tr;
    bl = vec(int(x),   int(y));
    br = vec(int(x)+1, int(y));
    tl = vec(int(x),   int(y)+1);
    tr = vec(int(x)+1, int(y)+1);

    // Gradient
    vec gbl, gbr, gtl, gtr;
    gbl = _g(bl);
    gbr = _g(br);
    gtl = _g(tl);
    gtr = _g(tr);

    // Relative
    vec rbl, rbr, rtl, rtr;
    rbl = p - bl;
    rbr = p - br;
    rtl = p - tl;
    rtr = p - tr;

    // Dot products
    float s, t, u, v;
    s = gbl.dot(rbl);
    t = gbr.dot(rbr);
    u = gtl.dot(rtl);
    v = gtr.dot(rtr);

    // Use a ease curve to bias extreme points
    float sx = 3 * (x - bl.x) * (x - bl.x) -
        2 * (x - bl.x) * (x - bl.x) * (x - bl.x);
    float a = s + sx * (t - s);
    float b = u + sx * (v - u);

    float sy = 3 * (y - bl.y) * (y - bl.y) -
        2 * (y - bl.y) * (y - bl.y) * (y - bl.y);
    return a + sy * (b - a);
}

vec Perlin::_g(vec v) const {
    int x = 36969*(int(v.x) & 65535) + (int(v.x) >> 16);
    int y = 18000*(int(v.y) & 65535) + (int(v.y) >> 16);
    float rand = float((x << 16) + y);
    float theta = rand * 2 * M_PI;
    return vec(cos(theta), sin(theta));
}
