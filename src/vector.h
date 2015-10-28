#ifndef VECTOR_H
#define VECTOR_H

#include <sstream>

struct vector3_t {
    double x, y, z;

    vector3_t() {
        x = y = z = 0;
    }

    vector3_t(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double magnitude() {
        return sqrt(x * x + y * y + z * z);
    }

    vector3_t normalize() {
        return *this * (1/magnitude());
    }

    std::string to_string() {
        std::ostringstream strs;
        strs << "<" << x << ", " << y << ", " << z << ">";
        return strs.str();
    }

    vector3_t operator+(const vector3_t &b) {
        return vector3_t(x + b.x, y + b.y, z + b.z);
    }

    vector3_t operator-(const vector3_t &b) {
        return vector3_t(x - b.x, y - b.y, z - b.z);
    }

    vector3_t operator*(double b) {
        return vector3_t(b * x, b * y, b * z);
    }
};

#endif
