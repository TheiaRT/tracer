#ifndef MATERIALS_H
#define MATERIALS_H

/* TODO: make pixel.h a new file */
#include "pnm_image.h"

struct color_t {
    double r, g, b, a;

    color_t() : r(0), g(0), b(0), a(0)
    {
    }

    color_t(double val) : r(val), g(val), b(val), a(val)
    {
    }

    color_t(double r, double g, double b, double a) : r(r), g(g), b(b), a(a)
    {
    }

    void operator+=(const color_t &other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        b += other.a;
    }

    color_t operator*(const color_t &other)
    {
        return color_t(r * other.r, g * other.g, b * other.b, a * other.a);
    }

    color_t operator/(double d)
    {
        return color_t(r / d, g / d, b / d, a / d);
    }

    /* Need to scale the doubles to ints. */
    pixel_t to_pixel(long denom)
    {
        return pixel_t(r * denom, g * denom, b * denom);
    }
};

struct material_t {
    color_t ambient;
    color_t diffuse;
    color_t specular;
    color_t emission;
    double shine;

    material_t() :
        ambient(color_t()),
        diffuse(color_t()),
        specular(color_t()),
        emission(color_t()),
        shine(0.0f)
    {

    }

    material_t(color_t ambient,
               color_t diffuse,
               color_t specular,
               color_t emission,
               double shine) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        emission(emission),
        shine(shine)
    {
    }
};

/* http://www.nicoptere.net/dump/materials.html */
const material_t BRASS = material_t(
    color_t(0.329412, 0.223529, 0.027451, 1),
    color_t(0.780392, 0.568627, 0.113725, 1),
    color_t(0.992157, 0.941176, 0.807843, 1),
    color_t(),
    27.8974
), BRONZE = material_t(
    color_t(0.2125, 0.1275, 0.054, 1),
    color_t(0.714, 0.4284, 0.18144, 1),
    color_t(0.393548, 0.271906, 0.166721, 1),
    color_t(),
    25.6
), CHROME = material_t(
    color_t(0.25, 0.25, 0.25, 1),
    color_t(0.4, 0.4, 0.4, 1),
    color_t(0.774597, 0.774597, 0.774597, 1),
    color_t(),
    76.8
), JADE = material_t(
    color_t(0.135, 0.2225, 0.1575, 0.95),
    color_t(0.54, 0.89, 0.63, 0.95),
    color_t(0.316228, 0.316228, 0.316228, 0.95),
    color_t(),
    12.8
), PEARL = material_t(
    color_t(0.25, 0.20725, 0.20725, 0.922),
    color_t(1, 0.829, 0.829, 0.922),
    color_t(0.296648, 0.296648, 0.296648, 0.922),
    color_t(),
    11.264
);

#endif
