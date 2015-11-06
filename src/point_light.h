#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "vector.h"
#include "material.h"
#include "scene_object.h"


class PointLight : public SceneObject {
public:
    PointLight(double x, double y, double z, material_t material);
    PointLight(vector3_t loc, material_t material);
    bool intersect_ray(ray_t ray, vector3_t &intersection_point);

private:
    vector3_t loc;
    void init(vector3_t loc, material_t material);
};

#endif
