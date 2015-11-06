#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <inttypes.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>

#include "pnm_image.h"
#include "vector.h"
#include "scene_object.h"

class Sphere : public SceneObject {
private:
    vector3_t loc;
    double radius;

public:
    Sphere(double x, double y, double z, double radius)
    {
        loc = vector3_t(x, y, z);
        this->radius = radius;
    }

    /*
      I learned how to check if a ray intersects a circle from here:
      http://www.purplealienplanet.com/node/20. If the ray and the sphere do not
      intersect, intersection_point is left untouched. If they do intersect, it
      is changed to the closest intersection point to the camera.
    */

    bool intersect_ray(ray_t ray, vector3_t &intersection_point)
    {
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
    RayTracer(std::vector<SceneObject *> scene);
    ~RayTracer();
    PnmImage render_image(size_t width, size_t height);

private:
    std::vector<SceneObject *> scene, lights;
    bool cast_ray(ray_t r, double &distance, material_t &material);
    color_t cast_shadow_rays(vector3_t intersection_point);
};

#endif
