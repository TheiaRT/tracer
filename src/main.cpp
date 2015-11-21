#include <stdio.h>
#include <vector>
#include <iostream>

#include "sphere.h"
#include "point_light.h"
#include "ray_tracer.h"
#include "material.h"
#include "parser.h"

using namespace std;

int main(int argc, char **argv)
{
    string scene_name = "";
    FILE *out_file = stdout;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            scene_name = string(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0) {
            out_file = fopen(argv[++i], "w+");
        }
    }

    Parser parser(scene_name);
    std::vector<SceneObject *> scene_objects;
    std::vector<SceneObject *> scene_lights;

    parser.parse_file(scene_objects, scene_lights);
    RayTracer tracer(scene_objects, scene_lights);
    PnmImage image = tracer.render_image(400, 400);

    for (SceneObject *s : scene_objects) {
        delete (Sphere *) s;
    }

    for (SceneObject *l : scene_lights) {
        delete (PointLight *) l;
    }

    if (out_file == NULL) {
        return 2;
    }

    if (image.write(out_file) == false) {
        return 3;
    }

    fclose(out_file);

    return 0;
}
