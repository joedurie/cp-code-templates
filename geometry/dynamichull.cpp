//Dynamic Convex Hull
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;
typedef complex<ll> pt;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define A(a) (a).begin(), (a).end()
#define CRS(a, b) (conj(a) * (b)).Y
#define X real()
#define Y imag()
#define N 100010

namespace std {
	bool operator<(pt a, pt b) { return a.X == b.X ? a.Y < b.Y : a.X < b.X; }
}

//helper function for dyn_in_hull
bool in(pt p, set<pt>& h) {
	if(h.empty() || p < *h.begin() || *h.rbegin() < p) return false;
	auto i = h.upper_bound(p), j = i--;
	return CRS(*j - p, *i - p) > 0; //change to >= if border counts as "inside"
}

//returns true if p contained in dynamic hull hu / hd
bool in_hull(pt p, set<pt>& hu, set<pt>& hd) { return in(p, hu) && in(-p, hd); }

//helper function for dyn_add
void fix_bad(set<pt>::iterator i, set<pt>&h, bool l) {
	if(i == --h.begin() || i == h.end()) return;
	pt p = *i; h.erase(p);
	if(!in(p, h)) h.insert(p);
	else fix_bad(l ? --h.lower_bound(p) : h.upper_bound(p), h, l);
}

//helper function for dyn_add_to_hull
void add(pt p, set<pt>& h) {
	if(in(p, h)) return;
	h.insert(p);
	fix_bad(--h.lower_bound(p), h, true);
	fix_bad(h.upper_bound(p), h, false);
}

//adds p to dynamic hull hu / hd
void add_to_hull(pt p, set<pt>& hu, set<pt>& hd) { add(p, hu), add(-p, hd); }

int main() {
    G(n) set<pt> hu, hd;
    F(i, 0, n) {
        G(x) G(y)
        add_to_hull({x, y}, hu, hd);
    }
}
