#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "scene_object.h"
#include "sphere.h"
#include "point_light.h"

/* This class exists for the sole purpose of bundling SceneObjects and
   PointLights. */
struct scene_t {
    scene_t()
        : objects(std::vector<SceneObject *>()),
          lights(std::vector<SceneObject *>())
    {
    }

    ~scene_t()
    {
        for (SceneObject *s : objects) {
            delete s;
        }

        for (SceneObject *l : lights) {
            delete (PointLight *) l;
        }
    }

    std::vector<SceneObject *> objects, lights;
};

#endif
