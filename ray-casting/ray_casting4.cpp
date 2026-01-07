#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
using namespace std;

struct Vec3 { double x,y,z; };
struct Ray { Vec3 orig, dir; };

struct Sphere {
    Vec3 center;
    double radius;
};

struct AABB {
    Vec3 min, max;
};

// Compute AABB of a sphere
AABB getSphereAABB(const Sphere &s) {
    return {
        {s.center.x - s.radius, s.center.y - s.radius, s.center.z - s.radius},
        {s.center.x + s.radius, s.center.y + s.radius, s.center.z + s.radius}
    };
}

// Ray-AABB intersection
bool intersectAABB(const Ray &r, const AABB &b) {
    double tmin = (b.min.x - r.orig.x) / r.dir.x;
    double tmax = (b.max.x - r.orig.x) / r.dir.x;
    if (tmin > tmax) swap(tmin, tmax);

    double tymin = (b.min.y - r.orig.y) / r.dir.y;
    double tymax = (b.max.y - r.orig.y) / r.dir.y;
    if (tymin > tymax) swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    double tzmin = (b.min.z - r.orig.z) / r.dir.z;
    double tzmax = (b.max.z - r.orig.z) / r.dir.z;
    if (tzmin > tzmax) swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    return true;
}

// Simple ray-sphere intersection
bool intersectSphere(const Ray &r, const Sphere &s, double &t) {
    double ocx = r.orig.x - s.center.x;
    double ocy = r.orig.y - s.center.y;
    double ocz = r.orig.z - s.center.z;
    double b = ocx*r.dir.x + ocy*r.dir.y + ocz*r.dir.z;
    double c = ocx*ocx + ocy*ocy + ocz*ocz - s.radius*s.radius;
    double disc = b*b - c;
    if (disc < 0) return false;
    t = -b - sqrt(disc);
    return t > 0;
}

struct BVHNode {
    AABB box;
    int left, right;
    int sphereIndex; // –1 if not leaf
};

vector<BVHNode> bvh;

// Build a very simple BVH by splitting in half
int buildBVH(int start, int end, vector<Sphere>& spheres) {
    int nodeIndex = bvh.size();
    bvh.push_back({});
    BVHNode &node = bvh.back();
    node.left = node.right = -1;
    node.sphereIndex = -1;

    // Compute enclosing AABB
    AABB bb = getSphereAABB(spheres[start]);
    for(int i = start+1; i < end; i++) {
        AABB sb = getSphereAABB(spheres[i]);
        bb.min.x = min(bb.min.x, sb.min.x);
        bb.min.y = min(bb.min.y, sb.min.y);
        bb.min.z = min(bb.min.z, sb.min.z);
        bb.max.x = max(bb.max.x, sb.max.x);
        bb.max.y = max(bb.max.y, sb.max.y);
        bb.max.z = max(bb.max.z, sb.max.z);
    }
    node.box = bb;

    int count = end - start;
    if (count == 1) {
        node.sphereIndex = start;
    } else {
        int mid = (start + end) / 2;
        node.left = buildBVH(start, mid, spheres);
        node.right = buildBVH(mid, end, spheres);
    }
    return nodeIndex;
}

// Traverse BVH for ray intersection
bool hitBVH(const Ray &r, int nodeIndex, vector<Sphere>& spheres) {
    BVHNode &node = bvh[nodeIndex];
    if (!intersectAABB(r, node.box)) return false;

    if (node.sphereIndex != -1) {
        double t;
        return intersectSphere(r, spheres[node.sphereIndex], t);
    }
    return hitBVH(r, node.left, spheres) || hitBVH(r, node.right, spheres);
}

int main() {
    vector<Sphere> spheres = { {{0,0,-5},1.0}, {{2,1,-7},1.2}, {{-1,-1,-4},0.8} };
    int root = buildBVH(0, spheres.size(), spheres);

    Ray r = {{0,0,0},{0,0,-1}};

    bool hit = hitBVH(r, root, spheres);
    cout << (hit ? "Ray hit something\n" : "Ray missed all\n");

    return 0;
}
