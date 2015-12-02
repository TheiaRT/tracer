#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "vector.h"
#include "material.h"
#include "scene_object.h"


class PointLight : public SceneObject {
public:
    PointLight(double x, double y, double z, double intensity);
    PointLight(vector3_t loc, double intensity);
    PointLight(vector3_t loc, color_t intensity);
    int intersect_ray(ray_t ray, double &distance);
    double distance_from(vector3_t point);
    vector3_t get_location();
    color_t get_intensity();

private:
    void init(vector3_t loc, color_t intensity);
};

#endif
