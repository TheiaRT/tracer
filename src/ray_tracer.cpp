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

    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            vector3_t start(x, y, -1000.0);
            vector3_t direction(0, 0, 0);
            ray_t ray(start, direction);
            image.set_pixel(x, y, cast_ray(ray));
        }
    }

    return image;
}

pixel_t RayTracer::cast_ray(ray_t r)
{
    return pixel_t();
}
