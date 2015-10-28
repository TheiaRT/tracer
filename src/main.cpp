#include <stdio.h>

#include "ray_tracer.h"

int main(int argc, char **argv)
{
    sphere_t sphere(0, 0, 0, 2);
    RayTracer r(&sphere, 1);
    PnmImage image = r.render_image(100, 100);

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
