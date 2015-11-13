#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <inttypes.h>
#include <cmath>
#include <iostream>

#include "pnm_image.h"
#include "vector.h"
#include "sphere.h"

struct sphere_t {
    vector3_t loc;
    double radius;

    sphere_t(double x, double y, double z, double radius) {
        loc = vector3_t(x, y, z);
        this->radius = radius;
    }
};

class RayTracer {
    public:
    RayTracer(sphere_t *scene, size_t num_objects);
    ~RayTracer();
    PnmImage render_image(size_t width, size_t height);
    pixel_t cast_ray(ray_t r);

    private:
    sphere_t *scene;
    size_t num_objects;
};

#endif
