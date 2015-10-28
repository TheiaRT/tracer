#include "pnm_image.h"

PnmImage::PnmImage(size_t width, size_t height, long denom) {
    this->width = width;
    this->height = height;
    this->denominator = denom;

    this->pixels = new pixel_t*[height];
    for (size_t i = 0; i < height; i++) {
        this->pixels[i] = new pixel_t[width];
    }
}

PnmImage::PnmImage(size_t width, size_t height) {
    PnmImage(width, height, 255);
}

PnmImage::~PnmImage() {
    for (size_t i = 0; i < height; i++) {
        delete [] this->pixels[i];
    }
    delete [] this->pixels;
}

PnmImage PnmImage::read(FILE *fp) {
    /* read from file tho */
    return PnmImage(100, 100, 255);
}

bool PnmImage::write(FILE *fp) {
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
