#include <vector>
#include <cmath>
#include <cassert>

#include "vector.h"
#include "pnm_image.h"
#include "ray_tracer.h"
#include "point_light.h"

#define MAX_DISTANCE INFINITY

RayTracer::RayTracer(std::vector<SceneObject *> scene,
                     std::vector<SceneObject *> lights)
{
    this->scene = scene;
    this->lights = lights;
}

RayTracer::~RayTracer()
{

}

/* Renders the scene into a PnmImage, casting rays. */
PnmImage RayTracer::render_image(size_t width, size_t height)
{
    PnmImage image(width, height);
    ray_t ray(vector3_t(0, 0, 0), vector3_t(0, 0, 1));

    for (size_t x = 0; x < width; x++) {
        ray.start.x = x;
        for (size_t y = 0; y < height; y++) {
            ray.start.y = y;
            double distance;
            material_t material;
            if (cast_ray(ray, distance, material)) {
                vector3_t intersection_point = ray.start * distance;
                color_t brightness = cast_shadow_rays(intersection_point);
                long denom = image.get_denominator();
                pixel_t pixel = (material.diffuse * brightness).to_pixel(denom);
                image.set_pixel(x, y, pixel);
            }
        }
    }

    return image;
}

/*
  TODO: make conical ray tracing
 */
bool RayTracer::cast_ray(ray_t ray, double &distance, material_t &material)
{
    double min_distance = MAX_DISTANCE;
    size_t num_objects = scene.size();
    for (size_t i = 0; i < num_objects; i++) {
        double temp_distance;
        if (scene[i]->intersect_ray(ray, temp_distance)) {
            if (temp_distance < min_distance) {
                min_distance = temp_distance;
                material = scene[i]->get_material();
            }
        }
    }

    if (min_distance < MAX_DISTANCE) {
        distance = min_distance;
        return true;
    }

    return false;
}

color_t RayTracer::cast_shadow_rays(vector3_t intersection_point) {
    color_t brightness_sum = color_t();
    size_t num_lights = lights.size();
    for (size_t i = 0; i < num_lights; i++) {
        PointLight *light = (PointLight *) lights[i];
        vector3_t light_loc = light->get_location();
        vector3_t direction = light_loc - intersection_point;
        ray_t shadow_ray(intersection_point, direction);

        double distance;
        material_t temp_material;
        bool in_shadow = cast_ray(shadow_ray, distance, temp_material);
        if (!in_shadow) {
            color_t intensity = light->get_intensity_percent();
            brightness_sum += intensity / (distance * distance);
        }
    }

    return brightness_sum;
}
