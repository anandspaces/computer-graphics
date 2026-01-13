#include <iostream>
#include <cmath>
using namespace std;

/* =======================
   CONSTANTS
   ======================= */
constexpr double PI = 3.14159265358979323846;

/* =======================
   VECTOR STRUCT
   ======================= */
struct Vec3 {
    double x, y, z;

    // Vector addition
    Vec3 operator+(const Vec3& b) const {
        return {x + b.x, y + b.y, z + b.z};
    }

    // Vector subtraction
    Vec3 operator-(const Vec3& b) const {
        return {x - b.x, y - b.y, z - b.z};
    }

    // Scalar multiplication
    Vec3 operator*(double s) const {
        return {x * s, y * s, z * s};
    }

    // Scalar division
    Vec3 operator/(double s) const {
        return {x / s, y / s, z / s};
    }

    // Component-wise multiplication (color math)
    Vec3 operator*(const Vec3& b) const {
        return {x * b.x, y * b.y, z * b.z};
    }

    // Unary minus
    Vec3 operator-() const {
        return {-x, -y, -z};
    }
};

/* =======================
   VECTOR MATH
   ======================= */
double dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

double length(const Vec3& v) {
    return sqrt(dot(v, v));
}

Vec3 normalize(const Vec3& v) {
    double len = length(v);
    if (len == 0.0) return {0,0,0};
    return v / len;
}

/* =======================
   LAMBERT SHADING
   ======================= */
// Pure diffuse (no specular)
Vec3 lambert(
    const Vec3& N,       // Surface normal
    const Vec3& L,       // Light direction
    const Vec3& albedo   // Surface color
) {
    Vec3 n = normalize(N);
    Vec3 l = normalize(L);

    // cos(theta)
    double NdotL = max(dot(n, l), 0.0);

    // Lambert BRDF = albedo / PI
    return albedo * (NdotL / PI);
}

/* =======================
   MAIN
   ======================= */
int main() {
    // Surface normal (pointing toward camera)
    Vec3 N = {0, 0, 1};

    // Light coming from top-right
    Vec3 L = {1, 1, 1};

    // Diffuse color (green)
    Vec3 albedo = {0.0, 1.0, 0.0};

    Vec3 color = lambert(N, L, albedo);

    cout << "Lambert color = ("
         << color.x << ", "
         << color.y << ", "
         << color.z << ")\n";

    return 0;
}
