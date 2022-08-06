#include<bits/stdc++.h>
#define ll long long
#define lll __int128
using namespace std;

const ll MAX = 5e5;
const double PI = acos(-1);


long double cal(long double a, long double b, long double alpha) {
    return (b * b * b * b / 2 + a * a * b * b) * alpha / 2
        - (long double)2.0 * a * b * b * b * sin(alpha) / 3
        - (long double)1.0 * a * a * a * a * tan(alpha) / (12 * cos(alpha) * cos(alpha));
    // long double area_plus
    //     = (long double)0.5 * b * b * alpha
    //     - (long double)0.5 * a * a * tan(alpha);
    // printf(">> %lf %lf\n", integral_plus, area_plus);

    // area += area_plus * 2;
}

int p[MAX];

long double solve() {
    long double integral = 0, area = 0;
    int n, a, b;
    scanf("%d%d%d", &n, &a, &b);
    for (int i = 0;i < n;i++) {
        scanf("%d", p + i);
    }
    p[n] = p[0] + 20000;
    for (int i = 0;i < n;i++) {
        long double alpha = (p[i + 1] - p[i]) * PI / 2e4;
        integral += cal(a, b, alpha);
        area += (long double)a * a * tan(alpha);
    }
    return integral * 2 / (PI * (long double)b * b - area);
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) printf("%.14llf\n", solve());
}