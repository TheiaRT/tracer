#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "pnm_image.h"
#include "vector.h"

struct color_t {
    double r, g, b, a;
};

struct material_t {
    color_t ambient;
    color_t diffuse;
    color_t specular;
    color_t emission;
    double shine;
};

/* Virtual Object Class. All 3d objects in our scene inherit from this class. */
class SceneObject {
  public:
    virtual bool intersect_ray(ray_t ray, vector3_t &point) = 0;

  protected:
    material_t material;
};

#endif
