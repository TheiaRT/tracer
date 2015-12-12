#include "box.h"


Box::Box(vector3_t min, vector3_t max, material_t material)
    : min(min), max(max)
{
    this->loc = vector3_t((min.x + max.x)/2,
                          (min.y + max.y)/2,
                          (min.z + max.z)/2);
    this->material = material;
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


    return OUTSIDE_HIT;
}

vector3_t Box::normal(vector3_t at_point)
{

    vector3_t normal(0, 0, 0);
    double xmin = min.x;
    double xmax = max.x;

    if (xmax < xmin) {
        std::swap(xmin, xmax);
    }
    double ymin = min.y;
    double ymax = max.y;

    if (ymax < ymin) {
        std::swap(ymin, ymax);
    }
    double zmin = min.z;
    double zmax = max.z;

    if (zmax < zmin) {
        std::swap(zmin, zmax);
    }

    /* Find which face the intersection point is on */
    if (std::abs(at_point.x - xmin) < 0.1) {
        normal.x = -1;
    }
    if (std::abs(at_point.x - xmax) < 0.1) {
        normal.x = 1;
    }
    if (std::abs(at_point.y - ymin) < 0.1) {
        normal.y = -1;
    }
    if (std::abs(at_point.y - ymax) < 0.1) {
        normal.y = 1;
    }
    if (at_point.z == zmin) {
        normal.z = -1;
    }
    if (at_point.z == zmax) {
        normal.z = 1;
    }
    return normal.normalize();
}
