#include <cmath>      // for sqrt()
#include <fstream>   // for file output
#include <iostream>  // for console output

/* =======================
   3D Vector class
   ======================= */
class Vec3 {
public:
    float x, y, z;

    // Constructor
    Vec3(float X=0, float Y=0, float Z=0) {
        x = X;
        y = Y;
        z = Z;
    }

    // Vector addition
    Vec3 add(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    // Vector subtraction
    Vec3 subtract(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    // Multiply vector by scalar
    Vec3 multiply(float s) const {
        return Vec3(x * s, y * s, z * s);
    }

    // Dot product
    float dot(const Vec3& v) const {
        return x*v.x + y*v.y + z*v.z;
    }

    // Normalize vector
    Vec3 normalize() const {
        float length = std::sqrt(dot(*this));
        return Vec3(x / length, y / length, z / length);
    }
};

/* =======================
   Ray class
   ======================= */
class Ray {
public:
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3& o, const Vec3& d) {
        origin = o;
        direction = d;
    }
};

/* =======================
   Sphere class
   ======================= */
class Sphere {
public:
    Vec3 center;
    float radius;
    Vec3 color;

    Sphere(const Vec3& c, float r, const Vec3& col) {
        center = c;
        radius = r;
        color = col;
    }

    // Ray–sphere intersection
    bool intersect(const Ray& ray, float& t) const {

        Vec3 oc = ray.origin.subtract(center);

        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;

        float discriminant = b*b - 4*a*c;
        if (discriminant < 0)
            return false;

        t = (-b - std::sqrt(discriminant)) / (2*a);
        return t > 0;
    }
};

/* =======================
   MAIN FUNCTION
   ======================= */
int main() {

    const int WIDTH = 400;
    const int HEIGHT = 300;

    // Camera position
    Vec3 camera(0, 0, 0);

    // One sphere in the scene
    Sphere sphere(
        Vec3(0, 0, -3),   // center
        0.6f,             // radius
        Vec3(1, 0, 0)     // red color
    );

    // Create PPM file
    std::ofstream image("easy_oop.ppm");
    image << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    // Loop over pixels
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WIDTH; x++) {

            // Convert pixel to viewport coordinates
            float u = (x + 0.5f) / WIDTH  - 0.5f;
            float v = (y + 0.5f) / HEIGHT - 0.5f;

            // Create ray direction
            Vec3 direction(u, v, -1);
            direction = direction.normalize();

            Ray ray(camera, direction);

            // Background color
            Vec3 color(0.2f, 0.3f, 0.5f);

            float t;

            // Ray–sphere intersection
            if (sphere.intersect(ray, t)) {

                // Compute hit point
                Vec3 hitPoint =
                    ray.origin.add(ray.direction.multiply(t));

                // Compute surface normal
                Vec3 normal =
                    hitPoint.subtract(sphere.center).normalize();

                // Visualize normal as color
                color =
                    normal.add(Vec3(1,1,1)).multiply(0.5f);
            }

            // Write pixel
            image << int(color.x * 255) << " "
                  << int(color.y * 255) << " "
                  << int(color.z * 255) << "\n";
        }
    }

    image.close();
    std::cout << "easy_oop.ppm generated successfully\n";
    return 0;
}
