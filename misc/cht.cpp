//Convex Hull Trick
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
#define A(a) (a).begin(), (a).end()
#define N 100010

struct seg { ld x; ll m, b; };
vector<seg> hull;

void insert(ll m, ll b) { //lines inserted in inc. slope order for maxhull, dec. for minhull
	while(hull.size()) {
		seg s = hull.back();
		if(s.b + s.m * s.x > b + m * s.x) { //< for minhull
			if(s.m - m) hull.push_back({(b - s.b) / (ld)(s.m - m), m, b});
			return;
		}
		hull.pop_back();
	}
	hull = {{LLONG_MIN, m, b}};
}

ll query(ll x) {
	seg s = *--upper_bound(A(hull), x, [](ll a, seg b) { return a < b.x; });
	return s.b + s.m * x;
}
