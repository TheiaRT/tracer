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
    pixel_t **render_pixel_chunk(size_t x,
                                 size_t y,
                                 size_t width,
                                 size_t height,
                                 size_t image_width,
                                 size_t image_height,
                                 long denom);

private:
    std::vector<SceneObject *> scene, lights;
    int cast_ray(ray_t r,
            double &distance,
            material_t &material,
            SceneObject *&obj,
            SceneObject *ignore);
    color_t calculate_illumination(vector3_t intersection_point,
            SceneObject *obj,
            vector3_t view_direction,
            int inside_obj,
            int refraction_index,
            int depth);
    color_t calculate_reflection(vector3_t intersection_point,
            vector3_t view_direction,
            SceneObject *obj,
            vector3_t normal,
            double cosI,
            double obj_refract_index,
            int depth);
    color_t calculate_refraction(vector3_t intersection_point,
            vector3_t view_direction,
            SceneObject *obj,
            vector3_t normal,
            double cosI,
            double obj_refract_index,
            int depth,
            int inside_obj);
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
