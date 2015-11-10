#ifndef SPHERE_H
#define SPHERE_H

#include "scene_object.h"


class Sphere : public SceneObject {
public:
    Sphere(vector3_t loc, material_t material, double radius);
    Sphere(double x, double y, double z, material_t material, double radius);
    bool intersect_ray(ray_t ray, double &distance);

private:
    vector3_t loc;
    double radius;

    void init(vector3_t loc, material_t material, double radius) {
        this->loc = loc;
        this->material = material;
        this->radius = radius;
    }
};

#endif
