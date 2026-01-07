#include <iostream>
#include <cmath>
using namespace std;

struct Vec3 {
    double x, y, z;
};

double dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 normalize(Vec3 v) {
    double m = sqrt(dot(v, v));
    return {v.x/m, v.y/m, v.z/m};
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return {a.x-b.x, a.y-b.y, a.z-b.z};
}

Vec3 scale(Vec3 v, double s) {
    return {v.x*s, v.y*s, v.z*s};
}

Vec3 reflect(Vec3 D, Vec3 N) {
    double k = 2 * dot(D, N);
    Vec3 t = scale(N, k);
    return subtract(D, t);
}

int main() {
    Vec3 rayO = {0, 2, 0};
    Vec3 rayD = normalize({0, -1, 1});

    Vec3 planeP = {0, 0, 0};
    Vec3 planeN = normalize({0, 1, 0});

    double denom = dot(rayD, planeN);
    if (fabs(denom) < 1e-6) {
        cout << "Ray parallel to plane\n";
        return 0;
    }

    double t = dot(subtract(planeP, rayO), planeN) / denom;
    if (t < 0) {
        cout << "Plane behind ray\n";
        return 0;
    }

    Vec3 hit = {
        rayO.x + rayD.x * t,
        rayO.y + rayD.y * t,
        rayO.z + rayD.z * t
    };

    Vec3 refl = normalize(reflect(rayD, planeN));

    cout << "Hit point: (" << hit.x << ", " << hit.y << ", " << hit.z << ")\n";
    cout << "Reflected direction: (" 
         << refl.x << ", " << refl.y << ", " << refl.z << ")\n";

    return 0;
}
