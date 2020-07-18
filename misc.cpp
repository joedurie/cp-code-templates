#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;
template<class T, class S>
ostream& operator << (ostream &o, const pair<T, S> &p) {
    return o << '(' << p.first << ", " << p.second << ')';
}
template<template<class, class...> class T, class... A>
typename enable_if<!is_same<T<A...>, string>(), ostream&>::type
operator << (ostream &o, T<A...> V) {
	o << '[';
	for(auto a : V) o << a << ", ";
	return o << ']';
}

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef complex<ld> pt;

#define G(x) ll x; cin >> x;
#define GD(x) ld x; cin >> x;
#define GS(s) string s; cin >> s;
#define F(i, l, r) for(ll i = l; i < r; i++)
#define FD(i, r, l) for(ll i = r; i > l; i--)
#define P(a, n) { cout << "{ "; F(_, 0, n) cout << a[_] << " "; cout << "}" << endl; }
#define CE(a, x) lower_bound(a.begin(), a.end(), x)
#define UP(a, x) upper_bound(a.begin(), a.end(), x)
#define FL(a, x) (x < *a.begin() ? a.end() : --UP(a, x))
#define LO(a, x) (x <= *a.begin() ? a.end() : --CE(a, x))
#define MC(a, x) a.lower_bound(x)
#define MU(a, x) a.upper_bound(x)
#define MF(a, x) (x < (*a.begin()).K ? a.end() : --MU(a, x))
#define ML(a, x) (x <= (*a.begin()).K ? a.end() : --MC(a, x))
#define EX(x) { cout << x << endl; exit(0); }
#define K first
#define V second
#define X real()
#define Y imag()
#define M 1000000007 //998244353
#define N 100010

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

//modular combinatorics (prime modulus)

#define INV(a) pw(a, M - 2) //inv a mod M (only works for prime M)
#define NCR(n, r) (f[n] * fi[r] % M * fi[n - r] % M)

ll f[N], fi[N];

ll pw(ll a, ll p) {
    if(!p) return 1;
    ll t = pw(a, p / 2);
    return t * t % M * (p % 2 ? a : 1) % M;
}

ll inv(ll a, ll b) { return 1 < a ? b - inv(b % a, a) * b / a : 1; } //inv a mod b

int main() {
    f[0] = fi[0] = 1;
    F(i, 1, N) f[i] = i * f[i - 1] % M, fi[i] = INV(f[i]);
}

//PBDS

#include <bits/extc++.h>
using namespace __gnu_cxx;
using namespace __gnu_pbds;

typedef tree<ll, null_type, less<ll>,
rb_tree_tag, tree_order_statistics_node_update> set_t;