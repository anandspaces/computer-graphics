#include <cmath>

struct Vec3 {
    float a, b, c;
};

Vec3 add(Vec3 x, Vec3 y) {
    return { x.a + y.a, x.b + y.b, x.c + y.c };
}

Vec3 sub(Vec3 x, Vec3 y) {
    return { x.a - y.a, x.b - y.b, x.c - y.c };
}

Vec3 mul(Vec3 x, float z) {
    return { x.a * z, x.b * z, x.c * z };
}

float dot(Vec3 x, Vec3 y) {
    return x.a * y.a + x.b * y.b + x.c * y.c;
}

Vec3 cross(Vec3 x, Vec3 y) {
    return {
        x.b * y.c - x.c * y.b,
        x.c * y.a - x.a * y.c,
        x.a * y.b - x.b * y.a
    };
}

Vec3 normalize(Vec3 x) {
    float length = std::sqrt(dot(x, x));
    if (length == 0.0f) return {0, 0, 0};
    return { x.a / length, x.b / length, x.c / length };
}
