#include "sphere.h"
#include <iostream>

#define MISS 0
#define OUTSIDE_HIT 1
#define INSIDE_HIT -1

Sphere::Sphere(vector3_t loc, material_t material, double radius)
{
    init(loc, material, radius);
}

Sphere::Sphere(double x, double y, double z, material_t material, double radius)
{
    init(vector3_t(x, y, z), material, radius);
}

/*
  I learned how to check if a ray intersects a circle from here:
  http://www.purplealienplanet.com/node/20. If the ray and the sphere do not
  intersect, intersection_point is left untouched. If they do intersect, it
  is changed to the closest intersection point to the camera.
*/
int Sphere::intersect_ray(ray_t ray, double &distance)
{
    if (radius == 0) {
        return MISS;
    }

    double A = ray.direction.dot(ray.direction);
    vector3_t dist = ray.start - loc;
    double B = 2 * ray.direction.dot(dist);
    double C = dist.dot(dist) - (radius * radius);
    double disc = B * B - 4 * A * C;


    if (disc < 0) {
        return MISS;
    }
    else {

        double dist1 = (-B - sqrt(disc))/(2*A); //closer intersection
        double dist2 = (-B + sqrt(disc))/(2*A); //farther intersection

        // if the closer distance is negative,
        // the intersection is from within the sphere
        if(dist1 < 0 && dist2 > 0) {
            distance = dist2;
            return INSIDE_HIT;
        } else {
            distance = dist1;
            return OUTSIDE_HIT;
        }

        return MISS;
    }
}
