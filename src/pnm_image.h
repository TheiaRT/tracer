#ifndef PNMIMAGE_H
#define PNMIMAGE_H

#include <cstdio>
#include <inttypes.h>

#include "pixel.h"


class PnmImage {
public:
    PnmImage(size_t width, size_t height, long denom);
    PnmImage(size_t width, size_t height);
    ~PnmImage();
    void init(size_t width, size_t height, long denom);
    static PnmImage read(FILE *fp);
    bool write(std::string filename);
    bool write(FILE *fp);
    bool set_pixel(size_t x, size_t y, pixel_t pixel);
    long get_denominator();
    void insert_chunk(pixel_t **chunk, size_t x, size_t y, size_t width, size_t height);

private:
    size_t width, height;
    long denominator;
    pixel_t **pixels;
};

#endif
