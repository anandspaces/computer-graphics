#include <cmath>      // for sqrt()
#include <fstream>   // for file output
#include <iostream>  // for console output
using namespace std;
/* =======================
   3D Vector structure
   ======================= */
struct Vec3 {
    float x, y, z;
};

/* Vector addition: a + b */
Vec3 add(Vec3 a, Vec3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

/* Vector subtraction: a - b */
Vec3 subtract(Vec3 a, Vec3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

/* Multiply vector by scalar */
Vec3 multiply(Vec3 v, float s) {
    return {v.x * s, v.y * s, v.z * s};
}

/* Dot product of two vectors */
float dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

/* Normalize a vector (make its length = 1) */
Vec3 normalize(Vec3 v) {
    float length = sqrt(dot(v, v));
    return {v.x / length, v.y / length, v.z / length};
}

/* =======================
   Ray structure
   ======================= */
struct Ray {
    Vec3 origin;      // where the ray starts
    Vec3 direction;   // direction the ray travels
};

/* =======================
   Sphere structure
   ======================= */
struct Sphere {
    Vec3 center;
    float radius;
    Vec3 color;       // RGB values (0 to 1)
};

/* =======================
   Ray–Sphere intersection
   ======================= */
bool intersectSphere(Ray ray, Sphere sphere, float &t) {

    // Vector from sphere center to ray origin
    Vec3 oc = subtract(ray.origin, sphere.center);

    // Coefficients of quadratic equation
    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;

    // Discriminant
    float discriminant = b*b - 4*a*c;

    // If discriminant < 0 → no intersection
    if (discriminant < 0)
        return false;

    // Compute nearest intersection distance
    t = (-b - sqrt(discriminant)) / (2 * a);

    // Intersection must be in front of camera
    return t > 0;
}

/* =======================
   Main function
   ======================= */
int main() {

    // Image resolution
    const int WIDTH = 400;
    const int HEIGHT = 300;

    // Camera position
    Vec3 camera = {0, 0, 0};

    // One sphere in the scene
    Sphere sphere = {
        {0, 0, -3},   // center
        0.6f,         // radius
        {1, 0, 0}     // color (red)
    };

    // Open PPM image file
    ofstream image("ray_casting.ppm");
    image << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    // Loop over every pixel
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WIDTH; x++) {

            // Convert pixel position to viewport coordinates
            float u = (x + 0.5f) / WIDTH - 0.5f;
            float v = (y + 0.5f) / HEIGHT - 0.5f;

            // Ray direction through pixel
            Vec3 direction = normalize({u, v, -1});

            // Create ray
            Ray ray = {camera, direction};

            // Default background color
            Vec3 color = {0.2f, 0.3f, 0.5f};

            float t;

            // Check intersection with sphere
            if (intersectSphere(ray, sphere, t)) {

                // Compute hit point
                Vec3 hitPoint = add(ray.origin,
                                    multiply(ray.direction, t));

                // Compute surface normal
                Vec3 normal = normalize(
                    subtract(hitPoint, sphere.center)
                );

                // Convert normal to color
                color = multiply(add(normal, {1,1,1}), 0.5f);
            }

            // Write pixel color to image
            image << int(color.x * 255) << " "
                  << int(color.y * 255) << " "
                  << int(color.z * 255) << "\n";
        }
    }

    image.close();
    cout << "ray_casting.ppm generated successfully\n";
}
