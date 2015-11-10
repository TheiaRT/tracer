#include <stdio.h>
#include <vector>

#include "sphere.h"
#include "point_light.h"
#include "ray_tracer.h"
#include "material.h"

int main(int argc, char **argv)
{
    std::vector<SceneObject *> scene_objects;
    scene_objects.push_back(new Sphere(200, 200, 100, CHROME, 100));
    scene_objects.push_back(new Sphere(0, 0, 100, JADE, 50));

    std::vector<SceneObject *> scene_lights;
    scene_lights.push_back(new PointLight(100, 100, 200, 255));

    RayTracer tracer(scene_objects, scene_lights);
    PnmImage image = tracer.render_image(400, 400);

    for (SceneObject *s : scene_objects) {
        delete (Sphere *) s;
    }

    for (SceneObject *l : scene_lights) {
        delete (PointLight *) l;
    }

    FILE *fp;
    if (argc == 1) {
        fp = stdout;
    }
    else if (argc == 2) {
        fp = fopen(argv[1], "w+");
    }
    else {
        return 1;
    }

    if (fp == NULL) {
        return 2;
    }

    if (image.write(fp) == false) {
        return 3;
    }

    fclose(fp);

    return 0;
}
