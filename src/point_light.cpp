#include "point_light.h"

PointLight::PointLight(double x, double y, double z, material_t material)
{
    init(vector3_t(x, y, z), material);
}

PointLight::PointLight(vector3_t loc, material_t material)
{
    init(loc, material);
}

void PointLight::init(vector3_t loc, material_t material)
{
    this->loc = loc;
    this->material = material;
}

bool PointLight::intersect_ray(ray_t ray, vector3_t &intersection_point) {
    return false;
}
