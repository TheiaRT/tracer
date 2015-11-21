#include <vector>
#include <cmath>
#include <cassert>

#include "vector.h"
#include "pnm_image.h"
#include "ray_tracer.h"
#include "point_light.h"

#define MAX_DISTANCE INFINITY
#define MAX_JUMPS 15

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
    ray_t ray(vector3_t(0, 0, -10), vector3_t(0, 0, 1));

    for (size_t x = 0; x < width; x++) {
        ray.start.x = x;
        for (size_t y = 0; y < height; y++) {
            ray.start.y = y;

            pixel_t pixel = pixel_t();
            double coef = 2000.0f;
            for (int level = 0; coef > 0.0f && level < MAX_JUMPS; level++) {
            //     double distance;
            //     SceneObject *object;
            //     if (cast_ray(ray, distance, object)) {
            //         vector3_t intersection_point = ray.start * distance;
            //         color_t brightness = cast_shadow_rays(object,
            //                                               intersection_point);
            //         long denom = image.get_denominator();
            //         material_t material = object->get_material();
            //         pixel += (material.diffuse * brightness).to_pixel(denom);
            //     }
                SceneObject *object;
                vector3_t intersection_point;
                ray_t next_ray;
                if (cast_ray(ray, object, intersection_point, next_ray)) {
                    color_t color = cast_shadow_rays(object,
                                                     intersection_point,
                                                     coef);
                    pixel += color.to_pixel(255);
                }
                else {
                    break;
                }
            }

            image.set_pixel(x, y, pixel);
        }
    }

    return image;
}

/* TODO: make conical ray tracing */
bool RayTracer::cast_ray(ray_t ray,
                         SceneObject *&object,
                         vector3_t &intersection_point,
                         ray_t &next_ray)
{
    double distance = MAX_DISTANCE;
    size_t num_objects = scene.size();
    for (size_t i = 0; i < num_objects; i++) {
        double temp_distance = MAX_DISTANCE;
        if (scene[i]->intersect_ray(ray, temp_distance)) {
            if (temp_distance < distance) {
                distance = temp_distance;
                object = scene[i];
            }
        }
    }

    if (distance < MAX_DISTANCE) {
        intersection_point = ray.start * distance;
        next_ray = ray_t()/* SOMETHING */;
        return true;
    }

    return false;
}

bool RayTracer::intersects_any_object(ray_t ray)
{
    SceneObject *object;
    vector3_t intersection_point;
    ray_t next_ray;
    return cast_ray(ray, object, intersection_point, next_ray);
}

color_t RayTracer::cast_shadow_rays(SceneObject *object,
                                    vector3_t intersection_point,
                                    double &coef)
{
    color_t brightness_sum = color_t();
    material_t material = object->get_material();
    size_t num_lights = lights.size();
    for (size_t i = 0; i < num_lights; i++) {
        PointLight *light = (PointLight *) lights[i];
        vector3_t light_loc = light->get_location();
        vector3_t direction = (light_loc - intersection_point).normalize();
        ray_t shadow_ray(intersection_point, direction);

        bool in_shadow = intersects_any_object(shadow_ray);
        if (!in_shadow) {
            vector3_t normal = object->normal_at_point(intersection_point);
            double lambert = direction.dot(normal) * coef;
            /* double distance = light_loc.distance_from(intersection_point); */
            color_t intensity = light->get_intensity();
            brightness_sum += intensity * material.diffuse * lambert;
            /* / (distance * distance); */
        }
    }

    coef *= material.reflectivity;

    // brightness_sum.r  *= (1e7);
    // brightness_sum.g  *= (1e7);
    // brightness_sum.b  *= (1e7);

    // if (brightness_sum.r > 1) {
    //     brightness_sum.r = 1;
    // }
    // if (brightness_sum.g > 1) {
    //     brightness_sum.g = 1;
    // }
    // if (brightness_sum.b > 1) {
    //     brightness_sum.b = 1;
    // }

    return brightness_sum;
}
