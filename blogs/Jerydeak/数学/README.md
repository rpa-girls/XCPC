# 1. FFT & NTT



## 详解链接

### 洛谷

https://www.luogu.com.cn/problem/solution/P3803

### OI-WIKI

FFT：https://oi-wiki.org/math/poly/fft/

NTT：https://oi-wiki.org/math/poly/ntt/



## 知识点概述

该算法用来在 $O(nlogn)$ 时间复杂度内计算多项式乘法。

总体步骤可以大致分为两个过程：

- 寻找 $n$ 个 $x_k$ 值，计算它们的函数值；
- 计算这 $n$ 个函数值两两的乘积，并执行逆运算即可得到 $f(x)g(x)$。

### FFT（快速傅里叶变换）

这 $n$ 个 $x$ 值即为单位根的幂。（复数群上）

### NTT（快速数论变换）

这 $n$ 个 $x$ 值即为原根的幂。（模数群上）



## 模板

### FFT

使用方法：

- 调用 init 函数，初始化 FFT 命名空间，其中两个参数分别为：
  - max_deg：整个运算中最大的指数，将根据该值生成 limit 和 limit 的高位 l；
  - rev_arr：逆位数组，只需要传一个空间即可（至少 limit 大小），初始化由 init 完成。
- fft 函数：
  - A：复数数组；
  - type：大于 0 表示正变换，否则表示逆变换；如果小于 0，则逆变换之后还会除以 limit。

```cpp
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
```

### NTT

使用方法：

- 调用 init 函数，初始化 NTT 命名空间，其中五个参数分别为：
  - max_deg：整个运算中最大的指数，将根据该值生成 limit 和 limit 的高位 l；
  - rev_arr：逆位数组，只需要传一个空间即可（至少 limit 大小），初始化由 init 完成；
  - CONST_P：模数（必须是素数）；
  - CONST_G：原根；
- ntt 函数：
  - A：long long 数组；
  - type：大于 0 表示正变换，否则表示逆变换；如果小于 0，则逆变换之后还会除以 limit。

```cpp
namespace NTT {
    typedef long long LL;
    int limit = 1, L, * r;
    int P, G, Gi;

    inline LL fastpow(LL a, LL k) {
        LL base = 1;
        while (k) {
            if (k & 1) base = (base * a) % P;
            a = (a * a) % P; k >>= 1;
        }
        return base % P;
    }

    inline void ntt(LL* A, int type) {
        for (int i = 0; i < limit; i++)
            if (i < r[i]) swap(A[i], A[r[i]]);
        for (int mid = 1; mid < limit; mid <<= 1) {
            LL Wn = fastpow((type > 0) ? G : Gi, (P - 1) / (mid << 1));
            for (int j = 0; j < limit; j += (mid << 1)) {
                LL w = 1;
                for (int k = 0; k < mid; k++, w = (w * Wn) % P) {
                    int x = A[j + k], y = w * A[j + k + mid] % P;
                    A[j + k] = (x + y) % P,
                        A[j + k + mid] = (x - y + P) % P;
                }
            }
        }
        if (type < 0) {
            LL inv = NTT::fastpow(limit, P - 2);
            for (int i = 0; i < limit; i++) A[i] = A[i] * inv % P;
        }
    }
    inline void init(int max_pow, int* rev_arr, int CONST_P, int CONST_G) {
        P = CONST_P, G = CONST_G, Gi = fastpow(G, P - 2);
        r = rev_arr;
        while (limit <= max_pow) limit <<= 1, L++;
        for (int i = 0; i < limit; i++) r[i] = (r[i >> 1] >> 1) | ((i & 1) << (L - 1));
    }
}
```

