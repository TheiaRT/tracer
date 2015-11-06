#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <inttypes.h>
#include <cmath>
#include <iostream>
#include <algorithm>

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

    /*
      I learned how to check if a ray intersects a circle from here:
      http://www.purplealienplanet.com/node/20. If the ray and the sphere do not
      intersect, intersection_point is left untouched. If they do intersect, it
      is changed to the closest intersection point to the camera.
    */

    bool intersect_ray(ray_t ray, vector3_t &intersection_point) {
        float A = ray.direction.dot(ray.direction);
        vector3_t dist = ray.start - loc;
        float B = 2 * ray.direction.dot(dist);
        float C = dist.dot(dist) - radius * radius;
        float disc = B * B - 4 * A * C;
        if (disc < 0) {
            return false;
        }
        else {
            float dist1 = -B + sqrt(B * B - 4 * A * C)/(2 * A);
            float dist2 = -B - sqrt(B * B - 4 * A * C)/(2 * A);
            float min_distance = std::min(dist1, dist2);
            intersection_point = ray.start * min_distance;

            return true;
        }
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
