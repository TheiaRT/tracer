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


class RayTracer {
public:
    RayTracer(std::vector<SceneObject *> scene,
              std::vector<SceneObject *> lights);
    ~RayTracer();
    PnmImage render_image(size_t width, size_t height);

private:
    std::vector<SceneObject *> scene, lights;
    bool cast_ray(ray_t r, color_t &color, ray_t &next_ray);
    bool intersects_any_object(ray_t ray);
    color_t cast_shadow_rays(SceneObject *object, vector3_t intersection_point);
};

#endif
