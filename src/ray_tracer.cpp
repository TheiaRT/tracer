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
    vector3_t eye = vector3_t(0, 0, -3);
    ray_t ray(vector3_t(0, 0, 0), vector3_t(0, 0, 1));
    // We project through a 4:3 viewport that scales with width and height,
    // Centered at 0,0,0
    for (size_t x = 0; x < width; x++) {
        ray.start.x = -2 + ((x/(double)width) * 4);
        for (size_t y = 0; y < height; y++) {
            ray.start.y = -1.5 + ((y/(double)height) * 3);
            ray.direction = (ray.start - eye).normalize();
            double distance;
            material_t material;
            SceneObject *obj;

            if (cast_ray(ray, distance, material, obj, NULL)) {
                vector3_t intersection_point = ray.start + 
                    (ray.direction * distance);
                color_t color = calculate_illumination(intersection_point,
                        obj,
                        ray.direction);
                long denom = image.get_denominator();
                pixel_t pixel = color.to_pixel(denom);
                image.set_pixel(x, y, pixel);
            }
        }
    }

    return image;
}

/*
TODO: make conical ray tracing
*/
bool RayTracer::cast_ray(ray_t ray, double &distance, 
        material_t &material, SceneObject *&object, SceneObject *ignore)
{
    double min_distance = MAX_DISTANCE;
    size_t num_objects = scene.size();
    for (size_t i = 0; i < num_objects; i++) {
        double temp_distance = MAX_DISTANCE;

        if (scene[i] != ignore && scene[i]->intersect_ray(ray, temp_distance)) {
            if (temp_distance < min_distance) {
                min_distance = temp_distance;
                material = scene[i]->get_material();
                object = scene[i];
            } 
        }
    }

    if (min_distance < MAX_DISTANCE && min_distance > 0) {
        distance = min_distance;
        return true;
    }
    return false;
}

color_t RayTracer::calculate_diffuse(
        SceneObject *obj,
        vector3_t intersection_point,
        PointLight *light)
{
    vector3_t light_loc = light->get_location();
    vector3_t light_direction = 
        (light_loc - intersection_point).normalize();
    vector3_t normal = (intersection_point - obj->get_location()).normalize();
    double distance = light_loc.distance_from(intersection_point);
    color_t intensity = light->get_intensity_percent() / (distance * distance);
    return intensity * normal.dot(light_direction);
}

color_t RayTracer::calculate_specular(
        SceneObject *obj,
        vector3_t intersection_point,
        PointLight *light,
        vector3_t view_dir)
{
    vector3_t light_loc = light->get_location();
    vector3_t light_direction = 
        (light_loc - intersection_point).normalize();
    material_t material = obj->get_material();
    vector3_t normal = (intersection_point - obj->get_location()).normalize();
    double reflection = normal.dot(light_direction) * 2.0;
    vector3_t phong_dir = (light_direction - normal) * reflection;
    double phong_term = phong_dir.dot(view_dir);
    if (phong_term > 1) {
        phong_term = 1;
    }
    double distance = light_loc.distance_from(intersection_point);
    color_t intensity = light->get_intensity_percent() / (distance * distance);
    phong_term = pow(phong_term, material.shine);
    return (phong_term < 0 ? color_t() : intensity * phong_term);
}

color_t RayTracer::calculate_illumination(
        vector3_t intersection_point,
        SceneObject *obj,
        vector3_t view_direction) {

    material_t obj_material = obj->get_material();
    color_t brightness_sum = obj_material.ambient;
    color_t diffuse_sum = color_t();
    color_t specular_sum = color_t();
    color_t reflection_sum = color_t();
    size_t num_lights = lights.size();
    for (size_t i = 0; i < num_lights; i++) {
        PointLight *light = (PointLight *) lights[i];
        vector3_t light_loc = light->get_location();
        vector3_t direction = light_loc - intersection_point;
        ray_t shadow_ray(intersection_point, direction.normalize());

        double distance = 0;
        material_t temp_material;
        SceneObject *temp_obj;
        bool in_shadow = cast_ray(shadow_ray,
                distance,
                temp_material,
                temp_obj, obj) || 
            shadow_ray.direction.dot(
                    (intersection_point-obj->get_location())
                    .normalize()) < 0;
        if (!in_shadow) {
            diffuse_sum  += calculate_diffuse(obj, intersection_point, light);

            specular_sum += calculate_specular(obj,
                    intersection_point,
                    light,
                    view_direction);
        }
    }

    brightness_sum += diffuse_sum  * obj_material.diffuse;
    brightness_sum += specular_sum * obj_material.specular;
    brightness_sum += reflection_sum * color_t(obj_material.shine);

    if (brightness_sum.r > 1) {
        brightness_sum.r = 1;
    }
    if (brightness_sum.g > 1) {
        brightness_sum.g = 1;
    }
    if (brightness_sum.b > 1) {
        brightness_sum.b = 1;
    }


    return brightness_sum;
}
