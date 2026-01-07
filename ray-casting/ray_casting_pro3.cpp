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
    double m = sqrt(dot(v,v));
    return {v.x/m, v.y/m, v.z/m};
}

Vec3 add(Vec3 a, Vec3 b) {
    return {a.x+b.x, a.y+b.y, a.z+b.z};
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return {a.x-b.x, a.y-b.y, a.z-b.z};
}

Vec3 scale(Vec3 v, double s) {
    return {v.x*s, v.y*s, v.z*s};
}

Vec3 reflect(Vec3 D, Vec3 N) {
    return subtract(D, scale(N, 2 * dot(D,N)));
}

bool refract(Vec3 D, Vec3 N, double n1, double n2, Vec3 &T) {
    double eta = n1 / n2;
    double cosI = -dot(N, D);
    double sinT2 = eta*eta*(1 - cosI*cosI);
    if (sinT2 > 1) return false; // Total internal reflection

    double cosT = sqrt(1 - sinT2);
    T = add(scale(D, eta), scale(N, eta*cosI - cosT));
    return true;
}

bool raySphere(Vec3 O, Vec3 D, Vec3 C, double r, double &t) {
    Vec3 OC = subtract(O, C);
    double b = 2 * dot(D, OC);
    double c = dot(OC, OC) - r*r;
    double disc = b*b - 4*c;
    if (disc < 0) return false;
    t = (-b - sqrt(disc)) / 2;
    return t > 0;
}

Vec3 trace(Vec3 O, Vec3 D, int depth) {
    if (depth <= 0) return {0,0,0};

    Vec3 sphereC = {0,0,-5};
    double sphereR = 1;
    double t;

    if (!raySphere(O,D,sphereC,sphereR,t))
        return {0.2, 0.4, 0.8}; // background

    Vec3 P = add(O, scale(D,t));
    Vec3 N = normalize(subtract(P, sphereC));

    Vec3 R = normalize(reflect(D, N));
    Vec3 reflColor = trace(P, R, depth-1);

    Vec3 T;
    Vec3 refrColor = {0,0,0};
    if (refract(D, N, 1.0, 1.5, T)) {
        T = normalize(T);
        refrColor = trace(P, T, depth-1);
    }

    return add(scale(reflColor, 0.5), scale(refrColor, 0.5));
}

int main() {
    Vec3 camera = {0,0,0};
    Vec3 dir = normalize({0,0,-1});

    Vec3 color = trace(camera, dir, 3);

    cout << "Final color: ("
         << color.x << ", "
         << color.y << ", "
         << color.z << ")\n";
}
