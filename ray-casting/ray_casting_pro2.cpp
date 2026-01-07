#include <iostream>
#include <cmath>
using namespace std;

struct Vec3 {
    double x, y, z;
};

// Vector subtraction: result = a - b
Vec3 subtract(Vec3 a, Vec3 b) {
    Vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

// Vector cross product
Vec3 crossProduct(Vec3 a, Vec3 b) {
    Vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

// Vector dot product
double dotProduct(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Ray–Triangle Intersection Test
bool rayTriangleIntersect(
    Vec3 O, Vec3 D,
    Vec3 A, Vec3 B, Vec3 C,
    Vec3 &hitPoint
) {
    const double EPS = 1e-8;

    // Compute triangle edges
    Vec3 AB = subtract(B, A);
    Vec3 AC = subtract(C, A);

    // Compute triangle normal
    Vec3 N = crossProduct(AB, AC);

    // Ray parallel to triangle?
    double denom = dotProduct(N, D);
    if (fabs(denom) < EPS)
        return false;

    // Compute intersection distance t
    Vec3 AO = subtract(A, O);
    double t = dotProduct(N, AO) / denom;
    if (t < 0)
        return false;

    // Compute intersection point P
    Vec3 P;
    P.x = O.x + D.x * t;
    P.y = O.y + D.y * t;
    P.z = O.z + D.z * t;

    // ---- Barycentric Coordinates ----
    Vec3 v0 = subtract(C, A);
    Vec3 v1 = subtract(B, A);
    Vec3 v2 = subtract(P, A);

    double d00 = dotProduct(v0, v0);
    double d01 = dotProduct(v0, v1);
    double d11 = dotProduct(v1, v1);
    double d20 = dotProduct(v2, v0);
    double d21 = dotProduct(v2, v1);

    double denomBC = d00 * d11 - d01 * d01;
    if (fabs(denomBC) < EPS)
        return false;

    double v = (d11 * d20 - d01 * d21) / denomBC;
    double w = (d00 * d21 - d01 * d20) / denomBC;
    double u = 1.0 - v - w;

    // Check if inside triangle
    if (u >= 0 && v >= 0 && w >= 0) {
        hitPoint = P;
        return true;
    }

    return false;
}

int main() {
    // Ray
    Vec3 O = {0, 0, 0};
    Vec3 D = {0.2, 0.3, 1.0};

    // Triangle
    Vec3 A = {0, 0, 5};
    Vec3 B = {1, 0, 5};
    Vec3 C = {0, 1, 5};

    Vec3 hit;

    if (rayTriangleIntersect(O, D, A, B, C, hit)) {
        cout << "Ray intersects triangle at:\n";
        cout << "x = " << hit.x << ", y = " << hit.y << ", z = " << hit.z << endl;
    } else {
        cout << "Ray does NOT intersect the triangle.\n";
    }

    return 0;
}
