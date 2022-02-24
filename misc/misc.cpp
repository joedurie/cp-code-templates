//Miscellaneous
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define A(a) (a).begin(), (a).end()
#define M 1000000007 //998244353
#define N 100010

//File Input / Output

freopen("in.txt", "r", stdin);
freopen("out.txt", "w", stdout);

//DSU

ll par[N];
ll dsu(ll i) { return par[i] ? par[i] = dsu(par[i]) : i; }

//Floyd-Warshall

ll n, d[N][N];

int main() {
    F(k, 0, n) F(i, 0, n) F(j, 0, n) if(d[i][k] - M && d[k][j] - M) //M = infinity
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

//Kadane's Algorithm

ll n, a[N];

int main() {
    ll mx = 0, cur = 0;
    F(i, 0, n) cur = max(0ll, cur + a[i]), mx = max(mx, cur);
    cout << mx << '\n';
}

//Time-in Time-out DFS

vector<ll> tree[N];
ll tIn[N], tOut[N];

ll dfs(ll i, ll p, ll t) {
    tIn[i] = tOut[i] = t;
    for(ll j : tree[i]) if(j - p) 
        tOut[i] = dfs(j, i, tOut[i] + 1);
    return tOut[i];
}

//Length of LIS (default is non-decreasing)

vector<ll> lis;
ll n, a[N];

int main() {
    F(i, 0, n) {
        if(lis.empty() || a[i] >= lis.back()) lis.push_back(a[i]); //change to > for strictly increasing
        else *upper_bound(A(lis), a[i]) = a[i]; //change to lower_bound for strictly increasing
    }
    cout << lis.size() << '\n';
}

//Modular Combinatorics

#define INV(a) pw(a, M - 2) //inv a mod M (only works for prime M)
#define NCR(n, r) (f[n] * fi[r] % M * fi[(n) - (r)] % M)

ll f[N], fi[N];

ll pw(ll a, ll p) { return p ? pw(a * a % M, p / 2) * (p & 1 ? a : 1) % M : 1; }

ll inv(ll a, ll b = M) { return 1 < a ? b - inv(b % a, a) * b / a : 1; } //inv a mod b

int main() {
    f[0] = fi[0] = 1;
    F(i, 1, N) f[i] = i * f[i - 1] % M, fi[i] = inv(f[i]);
}

//Prime Sieve + Phi Calculation

ll prime[N];

ll phi(ll n) {
    ll ans = n;
    while(n > 1) {
        ll p = prime[n];
        while(!(n % p)) n /= p;
        ans = ans / p * (p - 1);
    }
    return ans;
}

int main() {
    prime[0] = prime[1] = 1;
    F(i, 2, N) if(!prime[i]) {
        prime[i] = i;
        for (ll j = i * i; j < N; j += i) if(!prime[j]) prime[j] = i;
    }
}
