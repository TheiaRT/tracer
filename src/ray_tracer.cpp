#include "pnm_image.h"
#include "ray_tracer.h"

RayTracer::RayTracer(sphere_t *scene, size_t num_objects)
{
    this->scene = scene;
    this->num_objects = num_objects;
}

RayTracer::~RayTracer()
{

}

/*
 * Renders the scene into a PnmImage, casting rays.
 */
PnmImage RayTracer::render_image(size_t width, size_t height)
{
    PnmImage image(width, height);
    ray_t ray(vector3_t(0, 0, 0), vector3_t(0, 0, 1));

    for (size_t x = 0; x < width; x++) {
        ray.start.x = x;
        for (size_t y = 0; y < height; y++) {
            ray.start.y = y;
            image.set_pixel(x, y, cast_ray(ray));
        }
    }

    return image;
}

/*
  TODO: figure out which has the smallest distance
  TODO: make conical ray tracing
 */
pixel_t RayTracer::cast_ray(ray_t ray)
{
    for (size_t i = 0; i < num_objects; i++) {
        vector3_t ipoint;
        bool does_intersect = scene[i].intersect_ray(ray, ipoint);
        if (does_intersect) {
            return pixel_t(255, 255, 255);
        }
    }

    return pixel_t(0, 0, 0);
}
