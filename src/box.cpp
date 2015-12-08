#include "box.h"


Box::Box(vector3_t min, vector3_t max, material_t material)
    : min(min), max(max), material(material)
{
}

/* TODO: can be optimized. See MIT paper and Scratchapixel. */
int Box::intersect_ray(ray_t ray, double &distance)
{
    double tmin = (min.x - ray.start.x) / ray.direction.x;
    double tmax = (max.x - ray.start.x) / ray.direction.x;

    if (tmin > tmax) {
        std::swap(tmin, tmax);
    }

    double tymin = (min.y - ray.start.y) / ray.direction.y;
    double tymax = (max.y - ray.start.y) / ray.direction.y;

    if (tymin > tymax) {
        std::swap(tymin, tymax);
    }

    if ((tmin > tymax) || (tymin > tmax)) {
        return MISS;
    }

    if (tymin > tmin) {
        tmin = tymin;
    }

    if (tymax < tmax) {
        tmax = tymax;
    }

    double tzmin = (min.z - ray.start.z) / ray.direction.z;
    double tzmax = (max.z - ray.start.z) / ray.direction.z;

    if (tzmin > tzmax) {
        std::swap(tzmin, tzmax);
    }

    if ((tmin > tzmax) || (tzmin > tmax)) {
        return MISS;
    }

    if (tzmin > tmin) {
        tmin = tzmin;
    }

    if (tzmax < tmax) {
        tmax = tzmax;
    }

    distance = tmin;

    if (distance < 0) {
        distance = tmax;

        if (distance < 0) {
            return MISS;
        }
    }

    // std::cerr << "distance: " << distance << std::endl;

    return OUTSIDE_HIT;
}
