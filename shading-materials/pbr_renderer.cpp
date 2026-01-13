#include <iostream>
#include <cmath>
using namespace std;

/* =======================
   CONSTANTS
   ======================= */
constexpr double PI = 3.14159265358979323846;

/* =======================
   VECTOR
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

    Vec3 operator/(double s) const {
        return {x / s, y / s, z / s};
    }

    Vec3 operator*(const Vec3& b) const {
        return {x * b.x, y * b.y, z * b.z};
    }

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
    if (len == 0) return {0,0,0};
    return v * (1.0 / len);
}

/* =======================
   PBR FUNCTIONS
   ======================= */

// GGX Normal Distribution
double DistributionGGX(const Vec3& N, const Vec3& H, double roughness) {
    double a = roughness * roughness;
    double a2 = a * a;

    double NdotH = max(dot(N, H), 0.0);
    double NdotH2 = NdotH * NdotH;

    double denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

// Schlick-GGX Geometry
double GeometrySchlickGGX(double NdotV, double roughness) {
    double r = roughness + 1.0;
    double k = (r * r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

// Smith Geometry
double GeometrySmith(const Vec3& N, const Vec3& V, const Vec3& L, double roughness) {
    double NdotV = max(dot(N, V), 0.0);
    double NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) *
           GeometrySchlickGGX(NdotL, roughness);
}

// Fresnel (Schlick)
Vec3 FresnelSchlick(double cosTheta, const Vec3& F0) {
    return F0 + (Vec3{1,1,1} - F0) * pow(1.0 - cosTheta, 5.0);
}

/* =======================
   PBR SHADING
   ======================= */
Vec3 PBR(
    Vec3 N,
    Vec3 V,
    Vec3 L,
    Vec3 albedo,
    double metallic,
    double roughness
) {
    N = normalize(N);
    V = normalize(V);
    L = normalize(L);

    Vec3 H = normalize(V + L);

    Vec3 F0 = {0.04, 0.04, 0.04};
    F0 = F0 * (1.0 - metallic) + albedo * metallic;

    Vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    double D = DistributionGGX(N, H, roughness);
    double G = GeometrySmith(N, V, L, roughness);

    Vec3 numerator = F * D * G;
    double denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 1e-5;
    Vec3 specular = numerator * (1.0 / denom);

    Vec3 kS = F;
    Vec3 kD = (Vec3{1,1,1} - kS) * (1.0 - metallic);

    double NdotL = max(dot(N, L), 0.0);

    return (kD * albedo / PI + specular) * NdotL;
}

/* =======================
   MAIN
   ======================= */
int main() {
    Vec3 N = {0, 0, 1};
    Vec3 V = {0, 0, 1};
    Vec3 L = {1, 1, 1};

    Vec3 albedo = {1.0, 0.0, 0.0}; // red
    double metallic = 0.1;
    double roughness = 0.4;

    Vec3 color = PBR(N, V, L, albedo, metallic, roughness);

    cout << "PBR color = ("
         << color.x << ", "
         << color.y << ", "
         << color.z << ")\n";
}
