#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
inline int read()
{
    char c = getchar();int x = 0, f = 1;
    while (c < '0' || c>'9') { if (c == '-')f = -1;c = getchar(); }
    while (c >= '0' && c <= '9') { x = x * 10 + c - '0';c = getchar(); }
    return x * f;
}

namespace FFT {
    const double Pi = acos(-1.0);
    struct complex
    {
        double x, y;
        complex(double xx = 0, double yy = 0) { x = xx, y = yy; }
        complex operator + (complex b) { return complex(x + b.x, y + b.y); }
        complex operator - (complex b) { return complex(x - b.x, y - b.y); }
        complex operator * (complex b) { return complex(x * b.x - y * b.y, x * b.y + y * b.x); }
        complex operator / (double b) { return complex(x / b, y / b); }
    };
    int l;
    int* r;
    int limit = 1;
    inline void fft(complex* A, int type)
    {
        for (int i = 0;i < limit;i++)
            if (i < r[i]) swap(A[i], A[r[i]]);//求出要迭代的序列 
        for (int mid = 1;mid < limit;mid <<= 1)//待合并区间的中点
        {
            complex Wn(cos(Pi / mid), ((type > 0) ? 1 : -1) * sin(Pi / mid)); //单位根 
            for (int R = mid << 1, j = 0;j < limit;j += R)//R是区间的右端点，j表示前已经到哪个位置了 
            {
                complex w(1, 0);//幂 
                for (int k = 0;k < mid;k++, w = w * Wn)//枚举左半部分 
                {
                    complex x = A[j + k], y = w * A[j + mid + k];//蝴蝶效应 
                    A[j + k] = x + y;
                    A[j + mid + k] = x - y;
                }
            }
        }
        // 根据公式，逆推时还需要除以 n
        if (type < 0) for (int i = 0;i < limit;i++) A[i] = A[i] / limit;
    }
    inline void init(int max_deg, int* rev_arr) {
        r = rev_arr;
        while (limit <= max_deg) limit <<= 1, l++;
        for (int i = 0;i < limit;i++)
            r[i] = (r[i >> 1] >> 1) | ((i & 1) << (l - 1));
    }
}

const int MAXN = 1e7 + 10;
FFT::complex a[MAXN], b[MAXN];
int r[MAXN];
int main()
{
    int N = read(), M = read();
    for (int i = 0;i <= N;i++) a[i].x = read();
    for (int i = 0;i <= M;i++) b[i].x = read();
    FFT::init(M + N, r);
    FFT::fft(a, 1);
    FFT::fft(b, 1);
    for (int i = 0;i < FFT::limit;i++) a[i] = a[i] * b[i];
    FFT::fft(a, -1);
    for (int i = 0;i <= N + M;i++)
        printf("%d ", (int)(a[i].x + 0.5));
    return 0;
}