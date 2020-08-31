//Miscellaneous
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); i++)
#define A(a) (a).begin(), (a).end()
#define M 1000000007 //998244353
#define N 100010

//file input / output

freopen("in.txt", "r", stdin);
freopen("out.txt", "w", stdout);

//PBDS

#include <bits/extc++.h>
using namespace __gnu_cxx;
using namespace __gnu_pbds;

typedef tree<ll, null_type, less<ll>,
rb_tree_tag, tree_order_statistics_node_update> set_t;

int main() {
    set_t s;
    s.insert(6), s.insert(11), s.insert(15);
    cout << s.find_by_order(2) << ' ' << s.order_of_key(12) << '\n'; //prints 15 2
}

//DSU

ll par[N];
ll dsu(ll i) { return par[i] == i ? i : par[i] = dsu(par[i]); }

//Time-in-time-out DFS

vector<ll> tree[N];
ll tIn[N], tOut[N];

ll dfs(ll i, ll p, ll t) {
    tIn[i] = tOut[i] = t;
    for(ll j : tree[i]) if(j - p) 
        tOut[i] = dfs(j, i, tOut[i] + 1);
    return tOut[i];
}

//modular combinatorics

#define INV(a) pw(a, M - 2) //inv a mod M (only works for prime M)
#define NCR(n, r) (f[n] * fi[r] % M * fi[(n) - (r)] % M)

ll f[N], fi[N];

ll pw(ll a, ll p) {
    if(!p) return 1;
    ll t = pw(a, p / 2);
    return t * t % M * (p & 1 ? a : 1) % M;
}

ll inv(ll a, ll b) { return 1 < a ? b - inv(b % a, a) * b / a : 1; } //inv a mod b

int main() {
    f[0] = fi[0] = 1;
    F(i, 1, N) f[i] = i * f[i - 1] % M, fi[i] = INV(f[i]);
}

//prime sieve + phi calculation

ll prime[N];

ll phi(ll n) {
    ll ans = n;
    while(n > 1) {
        ll p = prime[n];
        while(!(n % p)) n /= p, ans *= p;
        ans = ans / p * (p-1);
    }
    return ans;
}

int main() {
    prime[0] = prime[1] = 1;
    F(i, 2, N) if(!prime[i]) {
        prime[i] = i;
        for (ll j = i * i; j < NN; j += i) if(!prime[j]) prime[j] = i;
    }
}

//Length of LIS (default is non-decreasing)

vector<ll> lis;
ll a[N];

int main() {
    G(n)
    F(i, 0, n) {
        cin >> a[i];
        if(lis.empty() || a[i] >= lis.back()) lis.push_back(a[i]); //change to > for strictly increasing
        else *upper_bound(A(lis), a[i]) = a[i]; //change to lower_bound for strictly increasing
    }
    cout << lis.size() << '\n';
}
