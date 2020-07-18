//Geometry
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef complex<ld> pt;
typedef pair<pt, pt> line;
typedef pair<pt, ld> circ;

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
//one point on the line / center of circle
#define P first
//direction vector of the line / radius of circle
#define D second
#define R second
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
//line through pt p with angle theta
#define ANG_LINE(p, theta) {p, polar(1, theta)}
//distance from pt p to line l(s)
#define DIST_TO(p, l, s) abs((p) - cl_pt_on_l(p, l, s))
//true if p is on (or close to) line l(s)
#define ON_LINE(p, l, s) (PARALLEL(l.P - p, l.D) && (!s || DOT(l.P - p, l.P + l.D - p) <= 0))
//pt p reflected over line l
#define REFLECT_PT(p, l) (2 * cl_pt_on_l(p, l, false) - (p))
//angle bisector (ray) of lines (rays) l1/l2
#define ANGLE_BIS(l1, l2) {l1.P, INF * (U(l1.D) + U(l2.D))}
//perpendicular bisector of line (segment) l
#define PERP_BIS(l) {l.P + l.D / 2., l.D * I}
//true if pt p is "above" line l (above may not be in traditional sense, but is consistent for a given l)
#define ABOVE_LINE(p, l) (CRS((p) - l.P, l.D) >= 0)
//puts convex poly formed by upper and lower hulls (as given by get_hulls) into hu
#define JOIN_HULLS(hu, hd) { hu.pop_back(); hd.pop_back(); for(pt p : hd) hu.push_back(p); }
//constants (INF and EPS may need to be modified)
ld PI = acos(-1), INF = 1e30, EPS = 0.00001;
pt I = {0, 1};

//intersection pt of l1(s1) and l2(s2), returns NULL if they are nonintersecting / parallel / identical
pt* intsct(line l1, bool s1, line l2, bool s2) {
    if(PARALLEL(l1.D, l2.D)) return NULL;
	ld t1 = CRS(l2.D, l2.P - l1.P) / CRS(l2.D, l1.D);
	ld t2 = CRS(l1.D, l1.P - l2.P) / CRS(l1.D, l2.D);
	if((s1 && (t1 < 0 || t1 > 1)) || (s2 && (t2 < 0 || t2 > 1))) return NULL;
	return l1.P + l1.D * t1;
}

//closest pt on l(s) to p
pt cl_pt_on_l(pt p, line l, bool s) {
	if(pt* q = intsct(l, sg, { p, l.D * pt(0, 1) }, false)) return *q;
	return abs(p - l.P) < abs(p - l.P - l.D) ? l.P : l.P + l.D;
}

//line r(ray) reflected off line l(s) (if no intersection, returns original ray)
line reflect_line(line r, line l, bool s) {
	pt* p = intsct(r, true, l, s);
	if(!p) return r;
	return { p, INF * (p - REFLECT_PT(r.P, l)) };
}

//convex hull of pts
pair<vector<pt>, vector<pt>> get_hulls(vector<pt> pts) {
	vector<pt> hu, hd;
	sort(pts.begin(), pts.end(), [](pt &a, pt &b) {
		return a.X == b.X ? a.Y < b.Y : a.X < b.X;
	});
	#define DO_HULL(h) for(auto p : pts) { \
		while(h.size() >= 2 && cross(h.back() - h[h.size() - 2], p - h[h.size() - 2]) <= 0) h.pop_back(); \
		h.push_back(p); }
	DO_HULL(hu)
	reverse(pts.begin(), pts.end());
	DO_HULL(hd)
	return {hu, hd};
}

//area of polygon, vertices in order (cw or ccw)
ld area(vector<pt> poly) {
	ld ans = CRS(poly.back(), poly[0]);
	for(ll i = 0; i < poly.size() - 1; i++) ans += CRS(poly[i], poly[i + 1]);
	return abs(ans / 2);
}

//perimeter of polygon, vertices in order (cw or ccw)
ld perim(vector<pt> poly) {
	ld ans = abs(poly.back() - poly[0]);
	for(ll i = 0; i < poly.size() - 1; i++) ans += abs(poly[i + 1] - poly[i]);
	return ans;
}

//centroid of polygon, vertices in order (cw or ccw)
pt centroid(vector<pt> poly) {
	pt cent = {0, 0}, p1 = poly.back();
	ld pArea = 0;
	for(ll i = 0; i < poly.size() - 1; i++) {
		pt p2 = poly[i], p3 = poly[(i + 1) % (n - 1)];
		ld tArea = CRS(p1, p2) + CRS(p2, p3) + CRS(p3, p1);
		cent += tArea * (p1 + p2 + p3) / 3;
		pArea += tArea;
	}
	return cent / pArea;
}

/* TODO:
intsctCC
intsctCL
common tangents of circles
is point inside poly?
streaming convex hull
fix onLine
circumcircle
incircle
orthocenter
*/