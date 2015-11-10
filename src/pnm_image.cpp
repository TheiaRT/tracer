#include <iostream>

#include "pnm_image.h"

PnmImage::PnmImage(size_t width, size_t height, long denom) {
    init(width, height, denom);
}

PnmImage::PnmImage(size_t width, size_t height) {
    init(width, height, 255);
}

PnmImage::~PnmImage() {
    for (size_t i = 0; i < height; i++) {
         delete [] this->pixels[i];
    }
    delete [] this->pixels;
}

void PnmImage::init(size_t width, size_t height, long denom) {
    this->width = width;
    this->height = height;
    this->denominator = denom;

    this->pixels = new pixel_t*[height];
    if (this->pixels == NULL) {
        std::cout << "NULL" << std::endl;
    }
    for (size_t i = 0; i < height; i++) {
        this->pixels[i] = new pixel_t[width];
        if (this->pixels[i] == NULL) {
            std::cout << "NULL" << std::endl;
        }
    }
}

PnmImage PnmImage::read(FILE *fp) {
    /* read from file tho */
    return PnmImage(100, 100, 255);
}

bool PnmImage::write(FILE *fp) {
    if (fp == NULL) {
        return false;
    }
    fprintf(fp, "P3\n");
    fprintf(fp, "%lu %lu\n", this->width, this->height);
    fprintf(fp, "%lu\n", this->denominator);
    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            pixel_t pix = this->pixels[x][y];
            fprintf(fp, " %lu %lu %lu", pix.r, pix.g, pix.b);
        }
        fprintf(fp, "\n");
    }
    return true;
}

bool PnmImage::set_pixel(size_t x, size_t y, pixel_t pixel) {
    if (x >= width || y >= height) {
        return false;
    }

    if (this->pixels == NULL || *(this->pixels) == NULL) {
        return false;
    }

    this->pixels[x][y] = pixel;
    return true;
}

long PnmImage::get_denominator()
{
    return denominator;
}
