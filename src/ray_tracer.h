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
#include "point_light.h"

class RayTracer {
public:
    RayTracer(std::vector<SceneObject *> scene,
              std::vector<SceneObject *> lights);
    ~RayTracer();
    PnmImage render_image(size_t width, size_t height);

private:
    std::vector<SceneObject *> scene, lights;
    bool cast_ray(ray_t r,
            double &distance,
            material_t &material,
            SceneObject *&obj,
            SceneObject *ignore);
    color_t calculate_illumination(vector3_t intersection_point,
            SceneObject *obj,
            vector3_t view_direction);
    color_t calculate_diffuse(
            SceneObject *obj,
            vector3_t intersection_point,
            PointLight *light);
    color_t calculate_specular(
            SceneObject *obj,
            vector3_t intersection_point,
            PointLight *light,
            vector3_t view_dir);
};

#endif
