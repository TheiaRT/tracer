#ifndef MATERIALS_H
#define MATERIALS_H

#include "util/pnm/pixel.h"
#include "util/pnm/pnm_image.h"

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
        a += other.a;
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
    double reflection;
    double refraction;
    double refraction_index;
    bool texture;

    material_t() :
        ambient(color_t()),
        diffuse(color_t()),
        specular(color_t()),
        emission(color_t()),
        reflection(0.0f),
        refraction(0),
        refraction_index(0),
        texture(false)
    {
    }

    material_t(color_t ambient,
               color_t diffuse,
               color_t specular,
               color_t emission,
               double reflection) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        emission(emission),
        reflection(reflection),
        refraction(0),
        refraction_index(1)
    {
    }

    material_t(color_t ambient,
               color_t diffuse,
               color_t specular,
               color_t emission,
               double reflection,
               double refraction,
               double refraction_index) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        emission(emission),
        reflection(reflection),
        refraction(refraction),
        refraction_index(refraction_index)
    {
    }
    material_t(color_t ambient,
               color_t diffuse,
               color_t specular,
               color_t emission,
               double reflection,
               double refraction,
               double refraction_index,
               bool texture) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        emission(emission),
        reflection(reflection),
        refraction(refraction),
        refraction_index(refraction_index),
        texture(texture)
    {
    }

    color_t get_texture(vector3_t point) {
        double modx = fabs(fmod(point.x, 1.0));
        double mody = fabs(fmod(point.y, 1.0));
        double modz = fabs(fmod(point.z, 1.0));

        if (texture) {
            /* Uncomment the following for checkerboard mode */
            /*if ((modx < 0.5) ^ (mody < 0.5)) {
                return color_t(0.1);
            } else {
                return color_t(1);
            } */
            return color_t(modx, mody, modz, 1);
        }
        return color_t(1);
    }
};

#endif
