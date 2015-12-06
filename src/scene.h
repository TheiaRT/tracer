#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "scene_object.h"
#include "sphere.h"
#include "point_light.h"


struct scene_t {
    scene_t()
        : objects(std::vector<SceneObject *>()),
          lights(std::vector<SceneObject *>())
    {
    }

    ~scene_t()
    {
        /* TODO: fix */
#if 0
        for (SceneObject *s : objects) {
            delete (Sphere *) s;
        }

        for (SceneObject *l : lights) {
            delete (PointLight *) l;
        }
#endif
    }

    std::vector<SceneObject *> objects, lights;
};

#endif
