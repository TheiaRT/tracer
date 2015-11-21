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
    /*
       scene_objects.push_back(new Sphere(50, 120, 50, JADE, 50));
       scene_objects.push_back(new Sphere(200, 120, 200, material_t(
       color_t(), color_t(1), color_t(), color_t(), 1), 100));
       */
    std::vector<SceneObject *> scene_lights;

    parser.parse_file(scene_objects, scene_lights);
    //scene_lights.push_back(new PointLight(vector3_t(0, 220, -50), 5255));
    cerr << ((PointLight *)scene_lights[0])->get_intensity_percent().r << endl;
    RayTracer tracer(scene_objects, scene_lights);
    PnmImage image = tracer.render_image(400, 400);

    for (SceneObject *s : scene_objects) {
        delete (Sphere *) s;
    }

    for (SceneObject *l : scene_lights) {
        delete (PointLight *) l;
    }
/*
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

*/
    if (out_file == NULL) {
        return 2;
    }

    if (image.write(out_file) == false) {
        return 3;
    }
    fclose(out_file);

    return 0;
}
