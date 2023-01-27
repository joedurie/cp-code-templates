//Suffix Tree
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define FD(i, r, l) for(ll i = r; i > (l); --i)
#define N 100010

map<char, ll> to[N], lk[N];
ll lf[N], rt[N], par[N], path[N];

#define att(a, b, c) to[par[a] = b][s[lf[a] = c]] = a;
void build(string s) {
	ll n = s.size(), z = 2;
	lf[1]--;
    FD(i, n - 1, -1) {
		ll v, q = n, o = z - 1, k = 0;
		for (v = o; !lk[v].count(s[i]) && v; v = par[v])
			q -= rt[path[k++] = v] - lf[v];
		ll w = lk[v][s[i]] + 1;
		if (to[w].count(s[q])) {
			ll u = to[w][s[q]];
			for (rt[z] = lf[u]; s[rt[z]] == s[q]; rt[z] += rt[v] - lf[v])
				v = path[--k], q += rt[v] - lf[v];
			att(z, w, lf[u])
			att(u, z, rt[z])
			lk[v][s[i]] = (w = z++) - 1;
		}
		lk[o][s[i]] = z - 1;
		att(z, w, q)
		rt[z++] = n;
	}
}
