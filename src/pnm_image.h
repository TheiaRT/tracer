#ifndef PNMIMAGE_H
#define PNMIMAGE_H

#include <cstdio>
#include <inttypes.h>

struct pixel_t {
    long r, g, b;

    pixel_t() {
        r = g = b = 0;
    }

    pixel_t(long r, long g, long b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class PnmImage {
public:
    PnmImage(size_t width, size_t height, long denom);
    PnmImage(size_t width, size_t height);
    ~PnmImage();
    static PnmImage read(FILE *fp);
    bool write(FILE *fp);
    bool set_pixel(size_t x, size_t y, pixel_t pixel);

    size_t width, height;
    long denominator;
    pixel_t **pixels;
};

#endif
