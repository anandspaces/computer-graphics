#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

/* =======================
   Vector math helpers
   ======================= */
struct Vec3 { float x,y,z; };

Vec3 add(Vec3 a, Vec3 b) {
    return {a.x+b.x, a.y+b.y, a.z+b.z};
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return {a.x-b.x, a.y-b.y, a.z-b.z};
}

Vec3 multiply(Vec3 v, float s) {
    return {v.x*s, v.y*s, v.z*s};
}

float dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 normalize(Vec3 v) {
    float len = sqrt(dot(v,v));
    return {v.x/len, v.y/len, v.z/len};
}

/* =======================
   Ray and Sphere
   ======================= */
struct Ray { Vec3 origin, direction; };

struct Sphere {
    Vec3 center;
    float radius;
    Vec3 color;
};

/* =======================
   Ray–Sphere intersection
   ======================= */
bool hitSphere(Ray ray, Sphere s, float &t) {

    Vec3 oc = subtract(ray.origin, s.center);

    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(oc, ray.direction);
    float c = dot(oc, oc) - s.radius*s.radius;

    float disc = b*b - 4*a*c;
    if (disc < 0) return false;

    t = (-b - sqrt(disc)) / (2*a);
    return t > 0;
}

int main() {

    const int WIDTH = 500;
    const int HEIGHT = 400;

    Vec3 camera = {0,0,0};
    Vec3 lightPos = {2,2,0};

    // Scene with multiple spheres
    vector<Sphere> scene = {
        {{0,0,-3}, 0.5, {1,0,0}},
        {{1,-0.2,-4}, 0.7, {0,1,0}}
    };

    ofstream image("ray_casting_pro.ppm");
    image << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    // Loop over pixels
    for(int y=HEIGHT-1; y>=0; y--) {
        for(int x=0; x<WIDTH; x++) {

            // Generate ray
            Vec3 dir = normalize({
                (x+0.5f)/WIDTH - 0.5f,
                (y+0.5f)/HEIGHT - 0.5f,
                -1
            });

            Ray ray = {camera, dir};

            float closest = 1e9;
            int hitIndex = -1;
            float tHit;

            // Find closest sphere hit
            for(int i=0;i<scene.size();i++) {
                float t;
                if(hitSphere(ray, scene[i], t) && t < closest) {
                    closest = t;
                    hitIndex = i;
                    tHit = t;
                }
            }

            // Background color
            Vec3 color = {0.1f, 0.1f, 0.1f};

            if(hitIndex != -1) {

                Sphere s = scene[hitIndex];

                // Compute hit point
                Vec3 hitPoint = add(ray.origin,
                                    multiply(ray.direction, tHit));

                // Surface normal
                Vec3 N = normalize(subtract(hitPoint, s.center));

                // Light direction
                Vec3 L = normalize(subtract(lightPos, hitPoint));

                // Shadow ray (offset to avoid self-intersection)
                Ray shadowRay = {
                    add(hitPoint, multiply(N, 0.001f)),
                    L
                };

                bool inShadow = false;

                // Shadow check
                for(auto obj : scene) {
                    float t;
                    if(hitSphere(shadowRay, obj, t)) {
                        inShadow = true;
                        break;
                    }
                }

                // Lambertian lighting
                float intensity = max(0.0f, dot(N, L));
                if(inShadow) intensity *= 0.2f;

                color = multiply(s.color, intensity);
            }

            // Write pixel
            image << int(color.x*255) << " "
                  << int(color.y*255) << " "
                  << int(color.z*255) << "\n";
        }
    }

    image.close();
    cout << "ray_casting_pro.ppm generated successfully\n";
}
