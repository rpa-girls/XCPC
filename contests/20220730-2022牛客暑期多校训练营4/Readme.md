# 总结

总共过了五个，ADHKN。

菜狗 yh 只过了 DKN 三个水水题，C 想用 NTT，但没调出来，DEBUG 太痛苦了。





# 题解

## A

### zhn

本题一定是选择一个最优的排序，我们可以根据最优情况下的相邻两个元素，得出偏序大小关系，之后用简单dp从后往前跑一下就好

```c++
#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N=1e5+8;
int n,m;
double ans[25];
struct Node{
    int w;
    double p;
}a[N];
inline int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch<'0'||ch>'9') {if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
    return x*f;
}
inline bool cmp(Node x,Node y){
    double x1=x.w+x.p*y.w;
    double x2=y.w+y.p*x.w;
    return x1<x2;
}
signed main(){
    n=read();m=read();
    for(int i=1;i<=n;i++) a[i].w=read();
    for(int i=1;i<=n;i++) {
        int q=read();
        a[i].p=(double)q/10000;
    }

    //按照奇怪的方式排序
    sort(a+1,a+n+1,cmp);
    memset(ans,0,sizeof(ans));
    for(int i=1;i<=n;i++){
        for(int j=m;j>=1;j--){
            ans[j]=max(ans[j],ans[j-1]*a[i].p+a[i].w);
        }
    }
    printf("%.8lf",ans[m]);
    return 0;
}

```



## B

### yh-补题

wrnm，tmd，精度他妈的有毛病。

double 全换 long double 就过了。

浪费我俩小时。

我日他妈。



## C

### yh-补题

tmd，我 TLE 了一万年，以为是预处理复杂度高了，结果是后面询问复杂度高了。

NTT 可做。



## D

### yh

简单 DP，tnnd，最开始想用线段树，发现题目读错了。

```cpp
#include<bits/stdc++.h>
#define ll long long
#define lll __int128
using namespace std;
template <typename _T>
inline _T Power(_T a, _T b, _T k)
{
    _T r = 1, base = a;
    while (b)
    {
        if (b & 1)
        {
            r *= base;
            r %= k;
        }
        base *= base;
        base %= k;
        b >>= 1;
    }
    return r;
}
const ll MOD = 998244353;
const ll INF = 0x7fffffff;

int m[12];
int min_val[12][401][401];
int dp[12][401][401];
int n, q;

int solve(int x, int y, int z) {
    int tot = 0;
    for (int i = 0;i < n;i++) {
        if (dp[i][x][y] <= z) tot++;
    }
    return tot;
}

int main() {
    scanf("%d%d", &n, &q);
    for (int i = 0;i < n;i++) {
        for (int x = 0;x <= 400;x++)
            for (int y = 0;y <= 400;y++) min_val[i][x][y] = dp[i][x][y] = INF;
        scanf("%d", m + i);
        for (int j = 0;j < m[i];j++) {
            int x, y, z;
            scanf("%d%d%d", &x, &y, &z);
            min_val[i][x][y] = min(min_val[i][x][y], z);
        }
        for (int x = 1;x <= 400;x++)
            for (int y = 1;y <= 400;y++) {
                dp[i][x][y] = min(dp[i][x - 1][y], dp[i][x][y - 1]);
                dp[i][x][y] = min(dp[i][x][y], min_val[i][x][y]);
            }
    }
    ll seed;
    scanf("%lld", &seed);
#include <random>
    std::mt19937 rng(seed);
    std::uniform_int_distribution<> u(1, 400);
    int lastans = 0;
    ll sum = 0;
    for (int i = 1;i <= q;i++)
    {
        int IQ = (u(rng) ^ lastans) % 400 + 1;  // The IQ of the i-th friend
        int EQ = (u(rng) ^ lastans) % 400 + 1;  // The EQ of the i-th friend
        int AQ = (u(rng) ^ lastans) % 400 + 1;  // The AQ of the i-th friend
        lastans = solve(IQ, EQ, AQ);  // The answer to the i-th friend
        // printf("%d %d %d -> %d\n", IQ, EQ, AQ, lastans);
        sum += lastans * Power((ll)seed, (ll)q - i, MOD) % MOD;
        sum %= MOD;
    }
    printf("%lld\n", (sum % MOD + MOD) % MOD);
}
```



## K

### yh

贪心，本质就是解一个同余方程，从小遍历 k 的可能就行了。

```cpp
#include<bits/stdc++.h>
#define ll long long
#define lll __int128
using namespace std;

template <typename _T>
inline _T Power(_T a, _T b)
{
    _T r = 1, base = a;
    while (b)
    {
        if (b & 1)
        {
            r *= base;
        }
        base *= base;
        b >>= 1;
    }
    return r;
}

const ll MAX = 5e5;

int main() {
    ll n;
    scanf("%lld", &n);
    ll times = 0;
    for (ll i = 1;i <= n;i++) {
        ll limit = 1;
        for (ll k = 0;;k++, limit *= 10ll) {
            ll x = i - (i - 1) * limit;
            x = (x % n) + n;
            x %= n;
            if (x < limit) {
                times += k;
                break;
            }
        }
    }
    printf("%lld", times);
}
```



## N

### yh

爆 long long！！！

tnnd

还是贪，注意变为与、或不改变某一位 0，1 的总数。

最后一定是：对每一位，从 0 到 1 依次排。

```cpp
#include<bits/stdc++.h>
#define ll long long
#define lll __int128
using namespace std;

const ll MAX = 5e5;

template <typename _T> _T Gcd(_T x, _T y) { return x ? Gcd(y % x, x) : y; }


void print(__int128 a) {
    if (a < 0) {
        putchar('-');
        a = -a;
    }
    if (a > 9) {
        print(a / 10);
    }
    putchar(a % 10 + '0');
}

ll cnt[15];

int main() {
    ll n;
    scanf("%lld", &n);
    lll sum = 0;
    for (int i = 0;i < n;i++) {
        ll a;
        scanf("%lld", &a);
        sum += a;
        for (int j = 0;j < 15 && a;j++, a /= 2) {
            cnt[j] += (a & 1);
        }
    }
    lll up = 0;

    for (ll i = 0;i < n;i++) {
        lll x = 0;
        ll m = 1;
        for (ll j = 0;j < 15;j++, m <<= 1) {
            if (i < cnt[j]) x |= m;
        }
        lll inner = n * x - sum;
        up += inner * inner;
    }
    lll down = ((lll)n) * n * n;
    if (up == 0) {
        down = 1;
    }
    else {

        lll g = Gcd(up, down);
        up /= g;
        down /= g;
    }
    print(up);
    putchar('/');
    print(down);
}
```

