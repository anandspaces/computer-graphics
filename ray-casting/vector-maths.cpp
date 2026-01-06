#include<cmath>
class Vec3 {
  float a,b,c;
}

Vec3 add(Vec3 x, Vec3 y) {
  return x.a + y.a, x.b + y.b, x.c + y.c; 
}

Vec3 sub(Vec3 x, Vec3 y) {
  return x.a - y.a, x.b - y.b, x.c - y.c;
}

Vec3 mul(Vec3 x, float z) {
  return x.a * z, x.b * z, x.c * z; 
}

float dot(Vec3 x, Vec3 y) {
  return x.a * y.a + x.b * y.b + x.c * y.c;
}

Vec3 cross(Vec3 x, Vec3 y) {
  return x.a * (y.a + y.b + y.c), -1 * x.b * (y.a + y.b + y.c), x.c * (y.a + y.b + y.c)
}

Vec3 normalize(Vec3 x) {
  float length = sqrt(dot(x,x))
  return x.a/length, x.b/length, x.c/length;
}

