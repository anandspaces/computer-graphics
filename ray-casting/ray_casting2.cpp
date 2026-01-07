#include <iostream>
using namespace std;

// Helper to compute area of triangle
float area2(float x1, float y1, float x2, float y2, float x3, float y3) {
    return abs(x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2));
}

int main() {
    float ax, ay, bx, by, cx, cy, px, py;
    cout << "Enter A(x y): "; cin >> ax >> ay;
    cout << "Enter B(x y): "; cin >> bx >> by;
    cout << "Enter C(x y): "; cin >> cx >> cy;
    cout << "Enter P(x y): "; cin >> px >> py;

    float A = area2(ax,ay,bx,by,cx,cy);
    float A1 = area2(px,py,bx,by,cx,cy);
    float A2 = area2(ax,ay,px,py,cx,cy);
    float A3 = area2(ax,ay,bx,by,px,py);

    float alpha = A1 / A;
    float beta  = A2 / A;
    float gamma = A3 / A;

    cout << "Barycentric coords: α=" << alpha << " β=" << beta << " γ=" << gamma << "\n";

    if(alpha>=0 && beta>=0 && gamma>=0 && alpha<=1 && beta<=1 && gamma<=1)
        cout << "Point P is inside the triangle.\n";
    else
        cout << "Point P is outside the triangle.\n";

    return 0;
}
