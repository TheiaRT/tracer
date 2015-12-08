#ifndef BOX_H
#define BOX_H

#include "scene_object.h"


class Box : public SceneObject {
public:
    Box(vector3_t min, vector3_t max, material_t material);
    int intersect_ray(ray_t ray, double &distance);
    vector3_t normal(vector3_t at_point) override;
private:
    vector3_t min, max;
};

#endif
