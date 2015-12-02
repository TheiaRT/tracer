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
using namespace std;
RayTracer::RayTracer(std::vector<SceneObject *> scene,
                     std::vector<SceneObject *> lights)
{
    this->scene = scene;
    this->lights = lights;
}

RayTracer::~RayTracer()
{

}

/**
 * Renders the scene using raytracing and renders it to a PnmImage of
 * dimensions width*height
 */
PnmImage RayTracer::render_image(size_t width, size_t height)
{

    PnmImage image(width, height);
    image.insert_chunk(
            render_pixel_chunk(0, 0, width, height/2, width, height, image.get_denominator()),
            0, 0, width, height/2);
    image.insert_chunk(
            render_pixel_chunk(0, height/2, width, height/2,
                               width, height,
                               image.get_denominator()),
            0, height/2, width, height/2);
    return image;

}

pixel_t **RayTracer::render_pixel_chunk(size_t startx,
                                        size_t starty,
                                        size_t chunk_width,
                                        size_t chunk_height,
                                        size_t image_width,
                                        size_t image_height,
                                        long denom) 
{
    pixel_t **pixels = new pixel_t*[chunk_height];
    for (size_t i = 0; i < chunk_height; i++) {
        pixels[i] = new pixel_t[chunk_width];
    }
    /* simulate the location of an eye */
    vector3_t eye = vector3_t(0, 0, -3); 
    ray_t ray(vector3_t(0, 0, 0), vector3_t(0, 0, 1));
    /* We project through a 4:3 viewport that scales with width and height,
     * Centered at 0,0,0 */
    for (size_t x = 0; x < chunk_width; x++) {
        ray.start.x = -(HORIZONTAL_ASPECT / 2)
                        + (((x + startx) / (double)image_width) * HORIZONTAL_ASPECT);
        for (size_t y = 0; y < chunk_height; y++) {
            ray.start.y = -(VERTICAL_ASPECT / 2)
                        + (((y + starty)/(double)image_height) * VERTICAL_ASPECT);
            ray.direction = (ray.start - eye).normalize();
            double distance;
            material_t material;
            SceneObject *obj;

            /* Call cast ray to find the first intersecting object */
            if (int result = cast_ray(ray, distance, material, obj, NULL)) {
                vector3_t intersection_point = ray.start +
                    (ray.direction * distance);
                 /* Recursively calculate the color of a pixel by calling 
                  * calculate illumination with the direction of the ray and
                  * the intersecting object. */
                color_t color = calculate_illumination(intersection_point,
                                                       obj,
                                                       ray.direction,
                                                       result,
                                                       REFRACTION_INDEX_AIR,
                                                       MAX_DEPTH);
                pixels[y][x]  = color.to_pixel(denom);
            }
        }
    }
    return pixels;
}

/*
 * Determine the closest intersecting object intersected by ray.
 * Set object equal to the intersected object, material equal
 * to the material of that object, and distance equal to the distance
 * between ray.start and the point of intersection.
 *
 * The return value is either
 *  1 if intersecting with the outside of a SceneObject.
 *  0 if not intersecting with a SceneObject
 *  -1 if intersecting with the inside of a SceneObject (for refractive objects)
 */
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

/*
 * Calculate diffuse shading value for obj at intersection point.
 */
color_t RayTracer::calculate_diffuse(SceneObject *obj,
                                     vector3_t intersection_point,
                                     PointLight *light)
{
    vector3_t light_loc = light->get_location();
    vector3_t light_direction =
        (light_loc - intersection_point).normalize();
    vector3_t normal = obj->normal(intersection_point);
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
    vector3_t normal = obj->normal(intersection_point);
    double reflection = normal.dot(light_direction) * 2.0;
    vector3_t phong_dir = light_direction - (normal * reflection);
    double phong_term = phong_dir.dot(view_dir);
    if (phong_term > 1) {
        phong_term = 1;
    }
    double distance = light_loc.distance_from(intersection_point);
    color_t intensity = light->get_intensity() / (distance * distance);
    phong_term = pow(phong_term, material.reflection + 1);
    return (phong_term < 0 ? color_t() : intensity * phong_term);
}

/*
 * Handle math for reflected rays, and recursively calculate illumination
 * of reflected_ray
 */
color_t RayTracer::calculate_reflection(vector3_t intersection_point,
                                        vector3_t incident_direction,
                                        SceneObject *obj,
                                        vector3_t normal,
                                        double cosI,
                                        double obj_refract_index,
                                        int depth)
{
    vector3_t reflection_direction = incident_direction - (normal * cosI * 2);
    ray_t reflected_ray(intersection_point, reflection_direction.normalize());
    SceneObject *reflection_obj = NULL;
    material_t material;

    double distance = 0;
    if (int inside = cast_ray(reflected_ray, distance, material,
        reflection_obj, obj) != 0) {
        vector3_t reflection_intersection = reflected_ray.start +
            (reflected_ray.direction * distance);
            return calculate_illumination(reflection_intersection,
                                                    reflection_obj,
                                                    reflected_ray.direction,
                                                    inside,
                                                    obj_refract_index,
                                                    depth-1) /
                                                    (distance * distance);
    }
    return color_t(0);
}

/*
 * Handle math for refracted rays, and recursively calculate illumination
 * of refracted_ray
 */
color_t RayTracer::calculate_refraction(vector3_t intersection_point,
                                        vector3_t incident_direction,
                                        SceneObject *obj,
                                        vector3_t normal,
                                        double cosI,
                                        double obj_refract_index,
                                        int depth,
                                        int inside_obj)
{
    material_t material;
    material_t obj_material = obj->get_material();
    SceneObject *refraction_obj;
    double distance = 0;

    double n = obj_refract_index/obj_material.refraction_index;
    cosI = -normal.dot(incident_direction);

    double cosT = 1.0 - n * n * (1.0 - cosI * cosI);
    vector3_t refraction_direction =
        incident_direction * n + normal * (n * cosI - sqrtf(cosT));

    ray_t refracted_ray(intersection_point,
        refraction_direction.normalize());

    if(cosT > 0.0) {
        if(inside_obj == -1) {
            obj = NULL;
        }
        if (int inside = cast_ray(refracted_ray, distance, material,
                refraction_obj, obj)) {

            vector3_t refraction_intersection = refracted_ray.start +
                    (refracted_ray.direction * distance);

            return calculate_illumination(refraction_intersection,
                                          refraction_obj,
                                          refracted_ray.direction,
                                          inside,
                                          obj_material.refraction_index,
                                          depth-1);
        }
    }
    return color_t(0);
}

/*
 * Recursively calculate the illumination of a point given an intersection
 * point, and an intersecting SceneObject obj
 */
color_t RayTracer::calculate_illumination(vector3_t intersection_point,
                                          SceneObject *obj,
                                          vector3_t incident_direction,
                                          int inside_obj,
                                          int refract,
                                          int depth)
{
    /* If depth is 0, stop reflecting ray and return base color */
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

    /* Calculate diffuse and specular lighting for each light in the scene, 
     * strinking intersection_point */
    for (size_t i = 0; i < num_lights; i++) {

        /* calculate direction from intersection point to light */
        PointLight *light = (PointLight *) lights[i];
        vector3_t light_loc = light->get_location();
        vector3_t direction = light_loc - intersection_point;
        ray_t shadow_ray(intersection_point, direction.normalize());

        double distance = 0;
        material_t temp_material;
        SceneObject *temp_obj = NULL;

        /* Determine if there are intersecting objects between
         * intersection_point and light in order to render shadow */
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
                    incident_direction);
        } else {
            /* if there is an intersecting object between intersection_point,
             * and light, scale the diffuse lighting by the refraction component
             * of the material. Opaque materials which do not refract light will
             * cast a darker shadow. */
            diffuse_sum  += calculate_diffuse(obj, intersection_point, light)
                                            * temp_material.refraction;
        }
    }

    SceneObject *refraction_obj = NULL;
    vector3_t normal = obj->normal(intersection_point) * inside_obj;
    /* cos-theta of incident ray */
    double cosI = normal.dot(incident_direction); 

    /* Call reflection and refraction helper functions to recursively Determine
     * accumulated brightness from refrected and refracted rays at
     * intersection_point */
    if(obj_material.reflection > 0) {
        reflection_sum += calculate_reflection(intersection_point,
                                               incident_direction,
                                               obj,
                                               normal,
                                               cosI,
                                               refract,
                                               depth);
    }

    if(obj_material.refraction > 0.0) {
        refraction_sum += calculate_refraction(intersection_point,
                                               incident_direction,
                                               obj,
                                               normal,
                                               cosI,
                                               refract,
                                               depth,
                                               inside_obj);
    }

    /* increment brightness by the diffuse lighting at intersection_point and
     * the accumulated sums of reflected and refracted rays. */
    brightness_sum += diffuse_sum  * obj_material.diffuse;
    brightness_sum += reflection_sum * color_t(obj_material.reflection);
    brightness_sum += refraction_sum * color_t(obj_material.refraction);

    brightness_sum = brightness_sum * obj_material.get_texture(intersection_point);


    /* Filter brightness_sum greater than 1 */
    
    brightness_sum.r = fmin(brightness_sum.r, 1);
    brightness_sum.g = fmin(brightness_sum.g, 1);
    brightness_sum.b = fmin(brightness_sum.b, 1);

    return brightness_sum;
}
