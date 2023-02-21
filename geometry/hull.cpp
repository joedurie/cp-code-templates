//Convex Hull
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
#define K first
#define V second
#define X real()
#define Y imag()
#define N 100010

namespace std {
	bool operator<(pt a, pt b) { return a.X == b.X ? a.Y < b.Y : a.X < b.X; }
}

bool in_hull(pt p, vector<pt>& hu, vector<pt>& hd) {
	if(p == *hu.begin() || p == *hd.begin()) return false; //change to true if border counts as inside
	if(p < *hu.begin() || *hd.begin() < p) return false;
	auto u = upper_bound(A(hu), p);
	auto d = lower_bound(hd.rbegin(), hd.rend(), p);
	return CRS(*u - p, *(u - 1) - p) > 0 && CRS(*(d - 1) - p, *d - p) > 0; //change to >= if border counts as "inside"
}

void do_hull(vector<pt>& pts, vector<pt>& h) {
	for(pt p : pts) {
		while(h.size() > 1 && CRS(h.back() - p, h[h.size() - 2] - p) <= 0) //change to < 0 if border points included
			h.pop_back();
		h.push_back(p);
	}
}

pair<vector<pt>, vector<pt>> get_hull(vector<pt>& pts) {
	vector<pt> hu, hd;
	sort(A(pts)), do_hull(pts, hu);
	reverse(A(pts)), do_hull(pts, hd);
	return {hu, hd};
}

vector<pt> full_hull(vector<pt>& pts) {
	auto h = get_hull(pts);
	h.K.pop_back(), h.V.pop_back();
	for(pt p : h.V) h.K.push_back(p);
	return h.K;
}

int main() {
    G(n) vector<pt> v;
    F(i, 0, n) {
        G(x) G(y)
        v.push_back({x, y});
    }
    vector<pt> h = full_hull(v);
}
