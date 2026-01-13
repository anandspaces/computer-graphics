#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

const double INF = 1e30;
const double EPS = 1e-6;

/* ---------------- BASIC STRUCTS ---------------- */

struct Vec3 {
    double x, y, z;
};

struct Ray {
    Vec3 origin;
    Vec3 dir;
};

struct Triangle {
    Vec3 v0, v1, v2;
};

struct AABB {
    Vec3 min;
    Vec3 max;
};

struct KdNode {
    AABB box;
    int left;
    int right;
    vector<int> triangles;
    bool isLeaf;
};

/* ---------------- GLOBAL STORAGE ---------------- */

vector<Triangle> gTriangles;
vector<KdNode> kdTree;

/* ---------------- UTILITY FUNCTIONS ---------------- */

double minDouble(double a, double b) {
    return (a < b) ? a : b;
}

double maxDouble(double a, double b) {
    return (a > b) ? a : b;
}

void swapInt(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void swapDouble(double &a, double &b) {
    double temp = a;
    a = b;
    b = temp;
}

/* ---------------- VECTOR MATH ---------------- */

Vec3 add(Vec3 a, Vec3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 scale(Vec3 v, double s) {
    return {v.x * s, v.y * s, v.z * s};
}

double dot(Vec3 a, Vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 cross(Vec3 a, Vec3 b) {
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

/* ---------------- RAY–TRIANGLE ---------------- */

bool rayTriangleIntersect(const Ray& ray, const Triangle& tri, double& t) {
    Vec3 e1 = subtract(tri.v1, tri.v0);
    Vec3 e2 = subtract(tri.v2, tri.v0);

    Vec3 p = cross(ray.dir, e2);
    double det = dot(e1, p);

    if (fabs(det) < EPS) return false;
    double invDet = 1.0 / det;

    Vec3 s = subtract(ray.origin, tri.v0);
    double u = invDet * dot(s, p);
    if (u < 0 || u > 1) return false;

    Vec3 q = cross(s, e1);
    double v = invDet * dot(ray.dir, q);
    if (v < 0 || u + v > 1) return false;

    t = invDet * dot(e2, q);
    return t > EPS;
}

/* ---------------- RAY–AABB ---------------- */

bool rayAABB(const Ray& ray, const AABB& box) {
    double tmin = (box.min.x - ray.origin.x) / ray.dir.x;
    double tmax = (box.max.x - ray.origin.x) / ray.dir.x;
    if (tmin > tmax) swapDouble(tmin, tmax);

    double tymin = (box.min.y - ray.origin.y) / ray.dir.y;
    double tymax = (box.max.y - ray.origin.y) / ray.dir.y;
    if (tymin > tymax) swapDouble(tymin, tymax);

    if (tmin > tymax || tymin > tmax) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    double tzmin = (box.min.z - ray.origin.z) / ray.dir.z;
    double tzmax = (box.max.z - ray.origin.z) / ray.dir.z;
    if (tzmin > tzmax) swapDouble(tzmin, tzmax);

    if (tmin > tzmax || tzmin > tmax) return false;

    return true;
}

/* ---------------- AABB COMPUTATION ---------------- */

AABB computeAABB(const vector<int>& indices) {
    AABB box;
    box.min = { INF, INF, INF };
    box.max = { -INF, -INF, -INF };

    for (int idx : indices) {
        const Triangle& t = gTriangles[idx];
        Vec3 verts[3] = {t.v0, t.v1, t.v2};
        for (int i = 0; i < 3; i++) {
            box.min.x = minDouble(box.min.x, verts[i].x);
            box.min.y = minDouble(box.min.y, verts[i].y);
            box.min.z = minDouble(box.min.z, verts[i].z);

            box.max.x = maxDouble(box.max.x, verts[i].x);
            box.max.y = maxDouble(box.max.y, verts[i].y);
            box.max.z = maxDouble(box.max.z, verts[i].z);
        }
    }
    return box;
}

/* ---------------- MANUAL SORT FOR KD-TREE ---------------- */

void sortTriangles(vector<int>& indices, int axis) {
    int n = indices.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Triangle& A = gTriangles[indices[j]];
            Triangle& B = gTriangles[indices[j + 1]];

            Vec3 ca = add(add(A.v0, A.v1), A.v2);
            Vec3 cb = add(add(B.v0, B.v1), B.v2);

            double va = (axis == 0 ? ca.x : (axis == 1 ? ca.y : ca.z));
            double vb = (axis == 0 ? cb.x : (axis == 1 ? cb.y : cb.z));

            if (va > vb) {
                swapInt(indices[j], indices[j + 1]);
            }
        }
    }
}

/* ---------------- KD-TREE BUILD ---------------- */

int buildKdTree(vector<int>& indices, int depth) {
    int nodeIndex = kdTree.size();
    kdTree.push_back({});
    KdNode& node = kdTree.back();

    node.box = computeAABB(indices);

    if (indices.size() <= 2 || depth >= 20) {
        node.isLeaf = true;
        node.triangles = indices;
        node.left = node.right = -1;
        return nodeIndex;
    }

    int axis = depth % 3;
    sortTriangles(indices, axis);

    int mid = indices.size() / 2;
    vector<int> leftList(indices.begin(), indices.begin() + mid);
    vector<int> rightList(indices.begin() + mid, indices.end());

    node.isLeaf = false;
    // node.triangles.clear();
    node.left = buildKdTree(leftList, depth + 1);
    node.right = buildKdTree(rightList, depth + 1);

    return nodeIndex;
}

/* ---------------- KD-TREE TRAVERSAL ---------------- */

bool traverseKd(const Ray& ray, int nodeIndex, double& closestT) {
    const KdNode& node = kdTree[nodeIndex];

    if (!rayAABB(ray, node.box))
        return false;

    bool hit = false;

    if (node.isLeaf) {
        for (int idx : node.triangles) {
            double t;
            if (rayTriangleIntersect(ray, gTriangles[idx], t)) {
                if (t < closestT) {
                    closestT = t;
                    hit = true;
                }
            }
        }
        return hit;
    }

    if (node.left >= 0)
        hit |= traverseKd(ray, node.left, closestT);
    if (node.right >= 0)
        hit |= traverseKd(ray, node.right, closestT);

    return hit;
}

/* ---------------- MAIN ---------------- */

int main() {
    gTriangles = {
        {{-1, -1, -5}, {1, -1, -5}, {1, 1, -5}},
        {{-1, -1, -5}, {1, 1, -5}, {-1, 1, -5}}
    };

    vector<int> indices;
    indices.push_back(0);
    indices.push_back(1);

    int root = buildKdTree(indices, 0);

    Ray ray;
    ray.origin = {0, 0, 0};
    ray.dir = {0, 0, -1};

    double closestT = INF;
    bool hit = traverseKd(ray, root, closestT);

    if (hit)
        cout << "Ray HIT geometry at t = " << closestT << endl;
    else
        cout << "Ray MISSED geometry\n";

    return 0;
}
