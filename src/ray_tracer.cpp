#include <vector>
#include <cmath>
#include <cassert>

#include "vector.h"
#include "pnm_image.h"
#include "ray_tracer.h"
#include "point_light.h"

#define MAX_DISTANCE INFINITY
#define MAX_DEPTH 10
#define REFRACTION_INDEX_AIR 1.0
#define REFRACTION_INDEX_WATER 1.3330
#define REFRACTION_INDEX_GLASS 1.52

#define HORIZONTAL_ASPECT 4
#define VERTICAL_ASPECT 3

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
        ray.start.x = -(HORIZONTAL_ASPECT/2) 
                        + ((x / (double)width) * HORIZONTAL_ASPECT);
        for (size_t y = 0; y < height; y++) {
            ray.start.y = -(VERTICAL_ASPECT/2) 
                        + ((y/(double)height) * VERTICAL_ASPECT);
            ray.direction = (ray.start - eye).normalize();
            double distance;
            material_t material;
            SceneObject *obj;

            if (int result = cast_ray(ray, distance, material, obj, NULL)) {
                vector3_t intersection_point = ray.start +
                    (ray.direction * distance);
                color_t color = calculate_illumination(intersection_point,
                        obj,
                        ray.direction,
                        result,
                        REFRACTION_INDEX_AIR,
                        MAX_DEPTH);
                long denom = image.get_denominator();
                pixel_t pixel = color.to_pixel(denom);
                image.set_pixel(x, y, pixel);
            }
        }
    }
    return image;
}

int RayTracer::cast_ray(ray_t ray,
                        double &distance,
                        material_t &material,
                        SceneObject *&object,
                        SceneObject *ignore)
{
    int dir;
    int min_dir;
    double min_distance = MAX_DISTANCE;
    size_t num_objects = scene.size();

    for (size_t i = 0; i < num_objects; i++) {
        double temp_distance = MAX_DISTANCE;
        if (scene[i] != ignore
            && (dir = scene[i]->intersect_ray(ray, temp_distance)) != 0) {
            if (temp_distance < min_distance) {
                min_dir = dir;
                min_distance = temp_distance;
                material = scene[i]->get_material();
                object = scene[i];
            }
        }
    }

    if (min_distance < MAX_DISTANCE && min_distance > 0) {
        distance = min_distance;
        return min_dir;
    }
    return false;
}

color_t RayTracer::calculate_diffuse(SceneObject *obj,
                                     vector3_t intersection_point,
                                     PointLight *light)
{
    vector3_t light_loc = light->get_location();
    vector3_t light_direction =
        (light_loc - intersection_point).normalize();
    vector3_t normal = (intersection_point - obj->get_location()).normalize();
    double distance = light_loc.distance_from(intersection_point);
    color_t intensity = light->get_intensity() / (distance * distance);
    return intensity * normal.dot(light_direction);
}

color_t RayTracer::calculate_specular(SceneObject *obj,
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
    vector3_t phong_dir = light_direction - (normal * reflection);
    double phong_term = phong_dir.dot(view_dir);
    if (phong_term > 1) {
        phong_term = 1;
    }
    double distance = light_loc.distance_from(intersection_point);
    color_t intensity = light->get_intensity() / (distance * distance);
    phong_term = pow(phong_term, material.reflection);
    return (phong_term < 0 ? color_t() : intensity * phong_term);
}

color_t RayTracer::calculate_illumination(vector3_t intersection_point,
                                          SceneObject *obj,
                                          vector3_t view_direction,
                                          int inside_obj,
                                          int refract,
                                          int depth) 
{

    if(depth <= 0) {
        return color_t(0);
    }

    material_t obj_material = obj->get_material();
    color_t brightness_sum = obj_material.ambient;
    color_t diffuse_sum = color_t();
    color_t specular_sum = color_t();
    color_t reflection_sum = color_t();
    color_t refraction_sum = color_t();
    size_t num_lights = lights.size();

    for (size_t i = 0; i < num_lights; i++) {
        PointLight *light = (PointLight *) lights[i];
        vector3_t light_loc = light->get_location();
        vector3_t direction = light_loc - intersection_point;
        ray_t shadow_ray(intersection_point, direction.normalize());

        double distance = 0;
        material_t temp_material;
        SceneObject *temp_obj = NULL;
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

    vector3_t normal =
        (intersection_point - obj->get_location()).normalize() * inside_obj;
    double cosI = normal.dot(view_direction); //cos-theta of incident ray
    vector3_t reflection_direction = view_direction - (normal * cosI * 2);
    ray_t reflected_ray(intersection_point, reflection_direction.normalize());
    SceneObject *reflection_obj = NULL;
    SceneObject *refraction_obj = NULL;
    material_t material;

    if(obj_material.reflection > 0) {
        double distance = 0;
        if (int result = cast_ray(reflected_ray, distance, material,
            refraction_obj, obj) != 0) {
            vector3_t reflection_intersection = reflected_ray.start +
                (reflected_ray.direction * distance);
                reflection_sum = calculate_illumination(reflection_intersection,
                                                        refraction_obj,
                                                        reflected_ray.direction,
                                                        result,
                                                        refract,
                                                        depth-1);
                reflection_sum = reflection_sum / (distance*distance);

        }
    }

    if(obj_material.refraction > 0.0) {
        double distance = 0;

        double n = refract/obj_material.refraction_index;
        normal = normal * inside_obj;
        cosI = -normal.dot(view_direction);
        double cosT = 1.0 - n * n * (1.0 - cosI * cosI);
        vector3_t refraction_direction =
            view_direction * n + normal * (n * cosI - sqrtf(cosT));

        ray_t refracted_ray(intersection_point,
            refraction_direction.normalize());

        if(cosT > 0.0) {
            if(inside_obj == -1) {
                obj = NULL;
            }
            if (int result = cast_ray(refracted_ray, distance, material,
                    refraction_obj, obj)) {

                vector3_t refraction_intersection = refracted_ray.start +
                        (refracted_ray.direction * distance);

                refraction_sum = calculate_illumination(refraction_intersection,
                                                        refraction_obj,
                                                        refracted_ray.direction,
                                                        result,
                                                        obj_material.refraction_index,
                                                        depth-1);
            }
        }
    }

    brightness_sum += diffuse_sum  * obj_material.diffuse;
    //brightness_sum += specular_sum * obj_material.specular;
    brightness_sum += reflection_sum * color_t(obj_material.reflection);
    brightness_sum += refraction_sum * color_t(obj_material.refraction);


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
