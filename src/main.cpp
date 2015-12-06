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
    int vp_width = 2000, vp_height = 1500;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            scene_name = string(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0) {
            out_file = fopen(argv[++i], "w+");
        }
        else if (strcmp(argv[i], "-vpw") == 0) {
            vp_width = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-vph") == 0) {
            vp_height = atoi(argv[++i]);
        }
    }

    Parser parser(scene_name);
    scene_t scene = parser.parse();
    RayTracer tracer(scene);
    PnmImage image = tracer.render_image(vp_width, vp_height);

    if (out_file == NULL) {
        return 2;
    }

    if (image.write(out_file) == false) {
        return 3;
    }

    fclose(out_file);

    return 0;
}
