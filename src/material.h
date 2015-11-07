#ifndef MATERIALS_H
#define MATERIALS_H

/* TODO: make pixel.h a new file */
#include "pnm_image.h"

struct color_t {
    double r, g, b, a;

    color_t()
    {
        r = g = b = a = 0;
    }

    color_t(double val)
    {
        r = g = b = a = val;
    }

    color_t(double r, double g, double b, double a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    void operator+=(const color_t &other)
    {
        this->r += other.r;
        this->g += other.g;
        this->b += other.b;
        this->b += other.a;
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
};

/* http://www.nicoptere.net/dump/materials.html */
const material_t BRASS = {
    .ambient  = color_t(0.329412, 0.223529, 0.027451, 1),
    .diffuse  = color_t(0.780392, 0.568627, 0.113725, 1),
    .specular = color_t(0.992157, 0.941176, 0.807843, 1),
    .shine    = 27.8974
}, BRONZE = {
    .ambient  = color_t(0.2125, 0.1275, 0.054, 1),
    .diffuse  = color_t(0.714, 0.4284, 0.18144, 1),
    .specular = color_t(0.393548, 0.271906, 0.166721, 1),
    .shine    = 25.6
}, CHROME = {
    .ambient  = color_t(0.25, 0.25, 0.25, 1),
    .diffuse  = color_t(0.4, 0.4, 0.4, 1),
    .specular = color_t(0.774597, 0.774597, 0.774597, 1),
    .shine    = 76.8
}, JADE = {
    .ambient  = color_t(0.135, 0.2225, 0.1575, 0.95),
    .diffuse  = color_t(0.54, 0.89, 0.63, 0.95),
    .specular = color_t(0.316228, 0.316228, 0.316228, 0.95),
    .shine    = 12.8
}, PEARL = {
    .ambient  = color_t(0.25, 0.20725, 0.20725, 0.922),
    .diffuse  = color_t(1, 0.829, 0.829, 0.922),
    .specular = color_t(0.296648, 0.296648, 0.296648, 0.922),
    .shine    = 11.264
};

#endif
