#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "vector.h"
#include "material.h"
#include "scene_object.h"


class PointLight : public SceneObject {
public:
    PointLight(double x, double y, double z, double intensity);
    PointLight(vector3_t loc, double intensity);
    bool intersect_ray(ray_t ray, double &distance);
    double distance_from(vector3_t point);
    vector3_t get_location();
    color_t get_intensity_percent();

private:
    vector3_t loc;
    void init(vector3_t loc, double intensity);
};

#endif
