#include "sphere.h"

Sphere::Sphere(vector3_t loc, material_t material, double radius)
{
    init(loc, material, radius);
}

Sphere::Sphere(double x, double y, double z, material_t material, double radius)
{
    init(vector3_t({x, y, z}), material, radius);
}

/*
  I learned how to check if a ray intersects a circle from here:
  http://www.purplealienplanet.com/node/20. If the ray and the sphere do not
  intersect, intersection_point is left untouched. If they do intersect, it
  is changed to the closest intersection point to the camera.
*/
bool Sphere::intersect_ray(ray_t ray, double &distance)
{
    if (radius == 0) {
        return false;
    }

    float A = ray.direction.dot(ray.direction);
    vector3_t dist = ray.start - loc;
    float B = 2 * ray.direction.dot(dist);
    float C = dist.dot(dist) - radius * radius;
    float disc = B * B - 4 * A * C;
    if (disc < 0) {
        return false;
    }
    else {
        float dist1 = -B + sqrt(B * B - 4 * A * C)/(2 * A);
        float dist2 = -B - sqrt(B * B - 4 * A * C)/(2 * A);
        distance = std::min(dist1, dist2);

        return true;
    }
}
