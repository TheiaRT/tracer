#include "pnm_image.h"
#include <iostream>
using namespace std;
int main() {
    PnmImage pnm(100, 100, 255);
    for (int x = 0; x < 100; x++) {
        pnm.set_pixel(x, x, pixel_t(255, 255, 255));
    }
    for (int x = 10; x < 100; x++) {
        pnm.set_pixel(x, 5, pixel_t(0, 255, 255));
    }
    pnm.write(stdout);
}
