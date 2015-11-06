#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "pnm_image.h"
#include "vector.h"
#include "material.h"

/* Virtual Object Class. All 3d objects in our scene inherit from this class. */
class SceneObject {
public:
    virtual bool intersect_ray(ray_t ray, vector3_t &point) = 0;

protected:
    material_t material;
};

#endif
