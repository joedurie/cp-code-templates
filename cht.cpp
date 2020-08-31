//Convex Hull Trick
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < r; i++)
#define N 100010
#define INF 10000000000000ll

struct seg { ld x; ll m, b; };
struct s_cmp { bool operator() (const seg& s, const seg& t) const { return s.x < t.x; } };

set<seg, s_cmp> hull;

void insert(ll m, ll b) { //lines inserted in inc. slope order for maxhull, desc. for minhull
	while(hull.size()) {
		seg s = *hull.rbegin();
		if(s.b + s.m * s.x <= b + m * s.x) hull.erase(s); //>= for minhull
		else break;
	}
	if(hull.empty()) { hull.insert({-INF, m, b}); return; }
	seg s = *hull.rbegin();
	if(s.m == m) return;
	ld x = (b - s.b) / (ld)(s.m - m);
	hull.insert({x, m, b});
}

ll query(ll x) {
	seg s = *--hull.upper_bound({(ld)x, INF, INF});
	return s.b + s.m * x;
}
