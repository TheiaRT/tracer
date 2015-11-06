#include <stdio.h>
#include <vector>

#include "ray_tracer.h"

int main(int argc, char **argv)
{
    std::vector<SceneObject *> scene;
    scene.push_back(new Sphere(200, 200, 100, 100));
    scene.push_back(new Sphere(0, 0, 100, 50));

    RayTracer tracer(scene);
    PnmImage image = tracer.render_image(400, 400);

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
