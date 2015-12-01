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
    for (size_t i = 0; i < height; i++) {
        this->pixels[i] = new pixel_t[width];
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
            pixel_t pix = this->pixels[y][x];
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

    this->pixels[y][x] = pixel;
    return true;
}

void PnmImage::insert_chunk(pixel_t **chunk, size_t startx, size_t starty, size_t width, size_t height)
{
    for (size_t x = 0; x < width; x++) {
        for (size_t y = 0; y < height; y++) {
            set_pixel(x + startx, y + starty, chunk[y][x]);
        }
    }
}
long PnmImage::get_denominator()
{
    return denominator;
}
