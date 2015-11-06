#include <stdio.h>

#include "ray_tracer.h"

int main(int argc, char **argv)
{
    sphere_t scene[] = {
        sphere_t(200, 200, 100, 100),
        sphere_t(0, 0, 100, 50)
    };

    RayTracer tracer(scene, 2);
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
