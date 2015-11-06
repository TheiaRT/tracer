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
    RayTracer(std::vector<SceneObject *> scene);
    ~RayTracer();
    PnmImage render_image(size_t width, size_t height);
    pixel_t cast_ray(ray_t r);

private:
    std::vector<SceneObject *> scene;
};

#endif
