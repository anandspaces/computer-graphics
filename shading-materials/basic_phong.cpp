#include <iostream>
#include <cmath>
using namespace std;

/* =======================
   BASIC VECTOR STRUCT
   ======================= */
struct Vec3 {
    double x, y, z;

    Vec3 operator+(const Vec3& b) const {
        return {x + b.x, y + b.y, z + b.z};
    }

    Vec3 operator-(const Vec3& b) const {
        return {x - b.x, y - b.y, z - b.z};
    }

    Vec3 operator*(double s) const {
        return {x * s, y * s, z * s};
    }

    // IMPORTANT: unary minus
    Vec3 operator-() const {
        return {-x, -y, -z};
    }
};

/* =======================
   VECTOR UTILITIES
   ======================= */
double dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

double length(const Vec3& v) {
    return sqrt(dot(v, v));
}

Vec3 normalize(const Vec3& v) {
    double len = length(v);
    if (len == 0) return {0, 0, 0};
    return v * (1.0 / len);
}

/* =======================
   REFLECTION
   R = L - 2(L·N)N
   ======================= */
Vec3 reflect(const Vec3& L, const Vec3& N) {
    return L - N * (2.0 * dot(L, N));
}

/* =======================
   PHONG SHADING
   ======================= */
Vec3 phong(
    Vec3 N,          // surface normal
    Vec3 L,          // light direction
    Vec3 V,          // view direction
    Vec3 lightColor,
    Vec3 ka,         // ambient coeff
    Vec3 kd,         // diffuse coeff
    Vec3 ks,         // specular coeff
    double shininess
) {
    // Normalize everything
    N = normalize(N);
    L = normalize(L);
    V = normalize(V);

    // Ambient
    Vec3 ambient = ka;

    // Diffuse
    double diff = max(0.0, dot(N, L));
    Vec3 diffuse = kd * diff;

    // Specular
    Vec3 R = reflect(-L, N);   // <- THIS needed unary minus
    double spec = pow(max(0.0, dot(R, V)), shininess);
    Vec3 specular = ks * spec;

    // Final color
    return (ambient + diffuse + specular) * 1.0;
}

/* =======================
   MAIN
   ======================= */
int main() {
    // Surface properties
    Vec3 N = {0, 0, 1};        // surface normal
    Vec3 V = {0, 0, 1};        // view direction

    // Light
    Vec3 L = {1, 1, 1};        // light direction
    Vec3 lightColor = {1, 1, 1};

    // Material
    Vec3 ka = {0.1, 0.1, 0.1}; // ambient
    Vec3 kd = {0.7, 0.2, 0.2}; // diffuse (red)
    Vec3 ks = {1.0, 1.0, 1.0}; // specular
    double shininess = 32.0;

    Vec3 color = phong(N, L, V, lightColor, ka, kd, ks, shininess);

    cout << "Phong color = ("
         << color.x << ", "
         << color.y << ", "
         << color.z << ")\n";

    return 0;
}
