#include<bits/stdc++.h>
#define ll long long
#define lll __int128
using namespace std;

inline int read() {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch>'9') { if (ch == '-')f = -1;ch = getchar(); }
    while (ch >= '0' && ch <= '9') { x = x * 10 + ch - '0';ch = getchar(); }
    return x * f;
}

namespace NTT {
    typedef long long LL;
    int limit = 1, L, * r, inv_limit;
    const int P = 998244353, G = 3, Gi = 332748118;

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
            for (int i = 0; i < limit; i++) A[i] = A[i] * inv_limit % P;
        }
    }
    inline void init(int max_pow, int* rev_arr, int CONST_P, int CONST_G) {
        // P = CONST_P, G = CONST_G, Gi = fastpow(G, P - 2);
        r = rev_arr;
        while (limit <= max_pow) limit <<= 1, L++;
        inv_limit = fastpow(limit, P - 2);
        for (int i = 0; i < limit; i++) r[i] = (r[i >> 1] >> 1) | ((i & 1) << (L - 1));
    }
}

const ll MAX = 8e5, NMAX = 1e7 + 4;
int w;
int c[12];
int N = 0;
int rev[MAX];
ll inv[NMAX], frac[NMAX], inv_frac[NMAX];
const ll MOD = 998244353;
ll val[12][MAX]; // val[k][t] 表示 k 个 f 相乘的 t 次项系数
ll tmp[12][MAX];
ll tmp2[MAX];

int main() {
    // freopen("c.input", "r", stdin);
    // freopen("c.output", "w+", stdout);
    w = read();
    for (int i = 0;i < w;i++) c[i] = read(), N += c[i];

    // Initialize
    NTT::init(N, rev, MOD, 3);
    inv[1] = frac[0] = inv_frac[0] = frac[1] = inv_frac[1] = 1;
    for (int i = 2;i < NMAX - 1;i++) {
        inv[i] = inv[MOD % i] * (MOD - MOD / i) % MOD;
        frac[i] = frac[i - 1] * i % MOD;
        inv_frac[i] = inv_frac[i - 1] * inv[i] % MOD;
    }
    val[0][0] = 1;

    // NTT to calculate val[][] by DP
    for (int i = 0;i < w;i++) {
        // 现在考虑 f_i 对 val 数组的影响
        // Construct f_i
        for (int j = 0;j < c[i];j++) tmp[i][j] = inv_frac[j];
        // for (int j = c[i];j <= N;j++) tmp[j] = 0;
        NTT::ntt(tmp[i], 1);
        // DP
        for (int j = w;j > 0;j--) {
            // Copy to tmp2
            // 疑问：为什么将下一行的 NTT::limit 改为 N，结果就不对了？
            for (int k = 0;k <= N;k++) tmp2[k] = val[j - 1][k];
            NTT::ntt(tmp2, 1);
            for (int k = 0;k < NTT::limit;k++) tmp2[k] = (tmp2[k] * tmp[i][k]) % MOD;
            NTT::ntt(tmp2, -1);
            for (int k = 0;k <= N;k++) val[j][k] = (tmp2[k] + val[j][k]) % MOD;
        }
        // printf("After Round %d\n", i);
        // for (int i = 0;i <= w;i++) {
        //     for (int j = 0;j <= N;j++) {
        //         printf("val[%d][%d] = %lld,  ", i, j, val[i][j]);
        //     }
        //     printf("\n");
        // }
    }

    // printf("Handle OK");

    // queries:
    int q = read();
    while (q--) {
        ll n = read();
        ll ans = 0;
        // 考虑每项 exp(x)^i * (-1)^{w-i} val[w-i]
        for (int i = 0;i <= w;i++) { // i: exp(x) 的次数
            ll ipow = NTT::fastpow(i, n);
            for (int j = 0;j <= N && j <= n;j++, ipow = (ipow * inv[i]) % MOD) { // j: 从 val[w-i] 中取出的 x 的次数
                // i^(n-j) / (n-j)! * val[w-i][j]
                if (n == j) ipow = 1; // to avoid error when n==j && i==0
                ll modifier = ipow * inv_frac[n - j] % MOD * val[w - i][j] % MOD;
                // printf("%d %d -> %lld*%lld*%lld = %lld\n", i, j, NTT::fastpow(i, n - j), inv_frac[n - j], val[w - i][j], modifier);
                if ((w - i) & 1) ans -= modifier;
                else ans += modifier;
            }
            ans %= MOD;
        }
        printf("%lld\n", ((ans * frac[n]) % MOD + MOD) % MOD);
    }
}
