#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "pnm_image.h"
#include "vector.h"
#include "material.h"

/* Virtual Object Class. All 3d objects in our scene inherit from this class. */
class SceneObject {
public:
    virtual int intersect_ray(ray_t ray, double &distance) = 0;
    material_t get_material() {
        return material;
    }

    vector3_t get_location() {
        return loc;
    }
    vector3_t normal(vector3_t at_point) {
        return (at_point - loc).normalize();
    }


protected:
    vector3_t loc;
    material_t material;
};

#endif
