#include "point_light.h"

PointLight::PointLight(double x, double y, double z, double intensity)
{
    init(vector3_t(x, y, z), intensity);
}

PointLight::PointLight(vector3_t loc, double intensity)
{
    init(loc, intensity);
}

void PointLight::init(vector3_t loc, double intensity)
{
    this->loc = loc;
    this->material = material_t();
    this->material.emission = color_t(intensity);
    /* white light */
    this->material.diffuse = color_t(1, 1, 1, 1);
}

bool PointLight::intersect_ray(ray_t ray, double &distance)
{
    return false;
}

double PointLight::distance_from(vector3_t point)
{
    return loc.distance_from(point);
}

vector3_t PointLight::get_location()
{
    return loc;
}

color_t PointLight::get_intensity_percent()
{
    return material.emission;
}
