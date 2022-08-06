#include<bits/stdc++.h>
using namespace std;

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

int N, M;
const int MAXN = 3 * 1e6 + 10;
long long a[MAXN], b[MAXN];
int r[MAXN];

int main() {
    scanf("%d%d", &N, &M);
    for (int i = 0; i <= N; i++) scanf("%lld", a + i);
    for (int i = 0; i <= M; i++) scanf("%lld", b + i);
    NTT::init(M + N, r, 998244353, 3);
    NTT::ntt(a, 1); NTT::ntt(b, 1);
    for (int i = 0; i < NTT::limit; i++) a[i] = (a[i] * b[i]) % NTT::P;
    NTT::ntt(a, -1);
    for (int i = 0; i <= N + M; i++)
        printf("%lld ", (a[i] % NTT::P + NTT::P) % NTT::P);
    return 0;
}