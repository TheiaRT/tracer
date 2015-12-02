#ifndef VECTOR_H
#define VECTOR_H

#include <sstream>
#include <math.h>

struct vector3_t {
    double x, y, z;

    vector3_t() : x(0), y(0), z(0)
    {
    }

    vector3_t(double x, double y, double z) : x(x), y(y), z(z)
    {
    }

    double magnitude()
    {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    vector3_t normalize()
    {
        return *this * (1/magnitude());
    }

    std::string to_string()
    {
        std::ostringstream strs;
        strs << "<" << x << ", " << y << ", " << z << ">";
        return strs.str();
    }

    vector3_t operator+(const vector3_t &b)
    {
        return vector3_t(x + b.x, y + b.y, z + b.z);
    }

    vector3_t operator-(const vector3_t &b)
    {
        return vector3_t(x - b.x, y - b.y, z - b.z);
    }

    vector3_t operator*(double b)
    {
        return vector3_t(b * x, b * y, b * z);
    }

    double dot(const vector3_t &b)
    {
        return x * b.x + y * b.y + z * b.z;
    }

    double distance_from(const vector3_t &b)
    {
        return (*this - b).magnitude();
    }
};

struct ray_t {
    vector3_t start, direction;

    ray_t(vector3_t start, vector3_t direction) :
        start(start), direction(direction)
    {
    }
};

#endif
