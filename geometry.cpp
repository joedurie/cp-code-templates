//Geometry
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef complex<ld> pt;
struct line {
	pt P, D; bool S;
	line s(bool b) { return {P, D, b}; }
};
struct circ { pt C; ld R; };

/**
 * builtin pt operations:
 * +, -: usual vector addition/subtraction
 * *, /: scalar multiplication/division (also complex multiplication / division if that's ever useful)
 * abs(p): vector length
 * norm(p): squared vector length
 * arg(p): angle p forms with positive x axis
 * polar(r, theta): vector with length r pointing in direction theta
 * conj(p): reflects p over x axis
 */

#define X real()
#define Y imag()
//line from two points
#define L2P(a, b) {a, b - a, 0};
//segment from two points
#define S2P(a, b) {a, b - a, 1}
//square of a number
#define SQ(x) ((x) * (x))
//scalar cross product of two pts
#define CRS(a, b) (conj(a) * (b)).Y
//dot product of two pts
#define DOT(a, b) (conj(a) * (b)).X
//pt p normalized to unit length
#define U(p) ((p) / abs(p))
//true if d1 and d2 (direction vectors) are parallel (or identical)
#define PARALLEL(d1, d2) (abs(CRS(U(d1), U(d2))) < EPS)
//line through pt p with angle th
#define ANG_LINE(p, th) {p, polar(1, th), 0}
//altitude from pt p to line l
#define ALT(p, l) L2P(p, cl_pt_on_l(l.s(0)))
//distance from pt p to line l
#define DIST_TO(p, l) abs((p) - cl_pt_on_l(p, l))
//true if p is on (or close to) line l
#define ON_LINE(p, l) (PARALLEL(l.P - p, l.D) && (!l.S || DOT(l.P - p, l.P + l.D - p) <= 0))
//pt p reflected over line l
#define REFLECT_PT(p, l) (2 * cl_pt_on_l(p, l.s(1)) - (p))
//angle bisector (ray) of angle abc
#define ANGLE_BIS(a, b, c) {b, INF * (U(a - b) + U(c - b)), 1}
//perpendicular bisector of segment l
#define PERP_BIS(l) {l.P + l.D / 2., l.D * I, 0}
//true if pt p is "above" line l (may not be in traditional sense, but consistent for fixed l)
#define ABOVE_LINE(p, l) (CRS((p) - l.P, l.D) > 0)
//puts convex poly formed by upper and lower hulls (as given by get_hulls) into hu
#define JOIN_HULLS(hu, hd) { hu.pop_back(); hd.pop_back(); for(pt p : hd) hu.push_back(p); }
//incenter of triangle abc
#define INCENT(a, b, c) *intsct(ANGLE_BIS(a, b, c), ANGLE_BIS(c, a, b))
//inradius of triangle abc
#define INRAD(a, b, c) DIST_TO(INCENT(a, b, c), S2P(a, b))
//circumcenter of triangle abc
#define CIRCUMCENT(a, b, c) *intsct(PERP_BIS(S2P(a, b)), PERP_BIS(S2P(a, c)))
//circumradius of triangle abc
#define CIRCUMRAD(a, b, c) abs((a) - CIRCUMCENT(a, b, c))
//orthocenter of triangle abc
#define ORTHOCENT(a, b, c) *intsct(ALT(a, S2P(b, c)), ALT(b, S2P(a, c)))
//comparator for sorting pts
constexpr bool operator<(const pt a, const pt b) {
	return a.X == b.X ? a.Y < b.Y : a.X < b.X;
}
//constants (INF and EPS may need to be modified)
ld PI = acos(-1), INF = 1e30, EPS = 0.00001;
pt I = {0, 1};

//intersection pt of l1 and l2, returns NULL if they are nonintersecting / parallel / identical
pt* intsct(line l1, line l2) {
    if(PARALLEL(l1.D, l2.D)) return NULL;
	pt p = l1.P + l1.D * CRS(l2.D, l2.P - l1.P) / CRS(l2.D, l1.D);
	if(!ON_LINE(p, l1) || !ON_LINE(p, l2)) return NULL;
	return &p;
}

//closest pt on l to p
pt cl_pt_on_l(pt p, line l) {
	if(pt* q = intsct(l, {p, l.D * I, 0})) return *q;
	return abs(p - l.P) < abs(p - l.P - l.D) ? l.P : l.P + l.D;
}

//line r(ray) reflected off line l(s) (if no intersection, returns original ray)
line reflect_line(line r, line l) {
	pt* p = intsct(r, l);
	if(!p) return r;
	return {p, INF * (p - REFLECT_PT(r.P, l)), 1};
}

//convex hull of pts (O(n) construction)
pair<vector<pt>, vector<pt>> get_hulls(vector<pt> pts) {
	vector<pt> hu, hd;
	sort(pts.begin(), pts.end());
	#define DO_HULL(h) for(auto p : pts) { \
		while(h.size() >= 2 && CRS(h.back() - h[h.size() - 2], p - h[h.size() - 2]) <= 0) h.pop_back(); \
		h.push_back(p); }
	DO_HULL(hu)
	reverse(pts.begin(), pts.end());
	DO_HULL(hd)
	return {hu, hd};
}

//is pt p in the convex hull given by pts
bool in_poly(set<pt> pts, pt p) {
	if(p < *pts.begin() || p > *pts.rbegin()) return true;
	auto q = --pts.upper_bound(p);
	return CRS(p - *q, *++q - p) > 0;
}

//area of polygon, vertices in order (cw or ccw)
ld area(vector<pt> poly) {
	ld ans = CRS(poly.back(), poly[0]);
	for(ll i = 1; i < poly.size(); i++) ans += CRS(poly[i - 1], poly[i]);
	return abs(ans / 2);
}

//perimeter of polygon, vertices in order (cw or ccw)
ld perim(vector<pt> poly) {
	ld ans = abs(poly.back() - poly[0]);
	for(ll i = 1; i < poly.size(); i++) ans += abs(poly[i] - poly[i - 1]);
	return ans;
}

//centroid of polygon, vertices in order (cw or ccw)
pt centroid(vector<pt> poly) {
	pt cent = {0, 0}, p1 = poly.back();
	ld pArea = 0;
	for(ll i = 0; i < poly.size() - 1; i++) {
		pt p2 = poly[i], p3 = poly[(i + 1) % (poly.size() - 1)];
		ld tArea = CRS(p1, p2) + CRS(p2, p3) + CRS(p3, p1);
		cent += tArea * (p1 + p2 + p3) / 3;
		pArea += tArea;
	}
	return cent / pArea;
}

//intersection pts of two circs (0, 1, or 2)
pair<pt*, pt*> intsctCC(circ c1, circ c2) {
	ld d = abs(c1.C - c2.C);
	if(d > c1.R + c2.R) return NULL;
	ld h = (SQ(d) - SQ(c2.R) + SQ(c1.R)) / (2 * d);
	pt v = U(I * (c2.P - c1.P)) * sqrt(SQ(d) - SQ(h));
	pt mid = (c1.C + c2.C) / 2.;
	pt p1 = mid + v, p2 = mid - v;
	return abs(p1 - p2) < EPS ? 
	pair<pt, pt> ps = {mid + v, mid - v};
	return &ps;
}

//intersection pts of a line and a circ (0, 1, or 2)
pair<pt*, pt*> intsctCL(circ c, line l) {
	if(DIST_TO(c.C, l) > c.R) return NULL;
	pt p = cl_pt_on_l(c.C, l);
	pt v = U(l.D) * sqrt(SQ(c.R) - SQ(abs(p - c.C)));
	pt p1 = p + v, p2 = p - v;
	pair<pt*, pt*> res = {&p1, &p2};
	if(!ON_LINE(p1, l)) res.first = NULL;
	if(!ON_LINE(p2, l) || abs(p1 - p2) < EPS) res.second = NULL;
	return res;
}

/* TODO:
make all sets/vectors pass by reference
common tangents of circles
streaming convex hull
golf everything (a lot)
lots of testing
*/