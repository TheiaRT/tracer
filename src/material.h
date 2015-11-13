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
