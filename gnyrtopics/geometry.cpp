//Geometry
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef complex<ld> pt;
struct line { pt P, D; bool S; };
struct circ { pt C; ld R; };

/**
 * builtin pt operations:
 * +, -: usual vector addition/subtraction
 * *, /: scalar multiplication/division (also complex multiplication / division if that's ever useful)
 * abs(p): vector length
 * norm(p): squared vector length
 * arg(p): angle p forms with positive x axis
 * polar(r, th): vector with length r pointing in direction th
 * conj(p): reflects p over x axis
 */

#define X real()
#define Y imag()
#define CRS(a, b) (conj(a) * (b)).Y //scalar cross product
#define DOT(a, b) (conj(a) * (b)).X //dot product
#define U(p) ((p) / abs(p)) //unit vector in direction of p (don't use if !p == true)
#define A(a) (a).begin(), (a).end() //shortens sort(), upper_bound(), etc. for vectors

//constants (INF and EPS may need to be modified)
constexpr ld PI = acos(-1), INF = 1e30, EPS = 0.0001;
constexpr pt I = {0, 1}, INV = {INF, INF};

namespace std {
	//true if p is within radius EPS of (0, 0)
	bool operator!(pt a) { return abs(a) < EPS; }
	//true if d is within EPS of 0
	bool operator!(ld d) { return abs(d) < EPS; }
	//lexicographical comparison
	bool operator<(pt a, pt b) { return !(a.X - b.X) ? a.Y < b.Y : a.X < b.X; }
}

/**
 * LINE DEFINITIONS
 */

//makes line l a full line (sets segment bool to false)
line ml(line l) { return {l.P, l.D, 0}; }

//line from two points
line l2p(pt p, pt q) { return {p, q - p, 0}; }

//segment from two points
line s2p(pt p, pt q) { return {p, q - p, 1}; }

//line through p with angle th
line ang_line(pt p, ld th) { return {p, polar(1ld, th), 0}; }

/**
 * GENERAL GEOMETRY FUNCTIONS
 */

//true if d1 and d2 parallel (zero vectors considered parallel to everything)
bool parallel(pt d1, pt d2) { return !d1 || !d2 || !CRS(U(d1), U(d2)); }

//"above" here means if l & p are rotated such that l.D points in the +x direction, then p is above l
bool above_line(pt p, line l) { return CRS(p - l.P, l.D) > 0; }

//true if p is on line l
bool on_line(pt p, line l) { return parallel(l.P - p, l.D) && (!l.S || DOT(l.P - p, l.P + l.D - p) <= 0); }

//intersection pt of l1 and l2, returns INV if they are nonintersecting / parallel / identical
pt intsct(line l1, line l2) {
	if(parallel(l1.D, l2.D)) return INV;
	pt p = l1.P + l1.D * CRS(l2.D, l2.P - l1.P) / CRS(l2.D, l1.D);
	return on_line(p, l1) && on_line(p, l2) ? p : INV;
}

//closest pt on l to p
pt cl_pt_on_l(pt p, line l) {
	pt q = l.P + DOT(U(l.D), p - l.P) * U(l.D);
	if(on_line(q, l)) return q;
	return abs(p - l.P) < abs(p - l.P - l.D) ? l.P : l.P + l.D;
}

//distance from p to l
ld dist_to(pt p, line l) { return abs(p - cl_pt_on_l(p, l)); }

//p reflected over l
pt refl_pt(pt p, line l) { return (ld)2 * cl_pt_on_l(p, ml(l)) - p; }

//ray r reflected off l (if no intersection, returns original ray)
line reflect_line(line r, line l) {
	pt p = intsct(r, l);
	if(!(p - INV)) return r;
	return {p, INF * (p - refl_pt(r.P, l)), 1};
}

/**
 * TRIANGLE CENTERS
 */

//altitude from p to l
line alt(pt p, line l) { return l2p(p, cl_pt_on_l(p, ml(l))); }

//angle bisector of angle abc
line ang_bis(pt a, pt b, pt c) { return {b, INF * (U(a - b) + U(c - b)), 1}; }

//perpendicular bisector of l (assumes l.S == 1)
line perp_bis(line l) { return {l.P + l.D / (ld)2, l.D * I, 0}; }

//orthocenter of triangle abc
pt orthocent(pt a, pt b, pt c) { return intsct(alt(a, s2p(b, c)), alt(b, s2p(a, c))); }

//incircle of triangle abc
circ incirc(pt a, pt b, pt c) {
	pt cent = intsct(ang_bis(a, b, c), ang_bis(b, a, c));
	return {cent, dist_to(cent, s2p(a, b))};
}

//circumcircle of triangle abc
circ circumcirc(pt a, pt b, pt c) {
	pt cent = intsct(perp_bis(s2p(a, b)), perp_bis(s2p(a, c)));
	return {cent, abs(cent - a)};
}

/**
 * CONVEX HULL
 */

//helper function for get_hull
void do_hull(vector<pt>& pts, vector<pt>& h) {
	for(pt p : pts) {
		while(h.size() > 1 && CRS(h.back() - p, h[h.size() - 2] - p) <= 0)
			h.pop_back();
		h.push_back(p);
	}
}

//returns upper convex hull / lower convex hull of pts
pair<vector<pt>, vector<pt>> get_hull(vector<pt>& pts) {
	vector<pt> hu, hd;
	sort(A(pts)), do_hull(pts, hu);
	reverse(A(pts)), do_hull(pts, hd);
	return {hu, hd};
}

//returns convex hull of pts as a vector of pts in cw order
vector<pt> full_hull(vector<pt>& pts) {
	auto h = get_hull(pts);
	h.first.pop_back(), h.second.pop_back();
	for(pt p : h.second) h.first.push_back(p);
	return h.first;
}

//returns true if p is contained in the convex hull given by hu / hd
bool in_hull(pt p, vector<pt>& hu, vector<pt>& hd) {
	if(p < *hu.begin() || *hd.begin() < p) return false;
	auto u = upper_bound(A(hu), p);
	auto d = lower_bound(hd.rbegin(), hd.rend(), p);
	return CRS(*u - p, *(u - 1) - p) > 0 && CRS(*(d - 1) - p, *d - p) > 0; //change to >= if border counts as "inside"
}

/**
 * DYNAMIC CONVEX HULL
 */

//helper function for in_dyn_hull
bool dyn_in(pt p, set<pt> h) {
	if(p < *h.begin() || *h.rbegin() < p) return false;
	auto i = h.upper_bound(p);
	return CRS(*i - p, *(i - 1) - p) > 0;
}

//returns true if p contained in dynamic hull hu / hd
bool in_dyn_hull(pt p, set<pt>& hu, set<pt>& hd) { return dyn_in(p, hu) && dyn_in(-p, hd); }

//helper function for add_to_dyn_hull
void dyn_add(pt p, set<pt> h, bool l = true, bool r = true) {
	h.erase(p);
	if(dyn_in(p, h)) return;
	h.insert(p);
	if(l && !(*h.begin() - p)) dyn_add(*(--h.lower_bound(p)), h, true, false);
	if(r && !(*h.rbegin() - p)) dyn_add(*h.upper_bound(p), h, false, true);
}

//adds p to dynamic hull hu / hd
void add_to_dyn_hulls(pt p, set<pt>& hu, set<pt>& hd) { dyn_add(p, hu), dyn_add(-p, hd); }

vector<pt> full_hull_from_dyn(set<pt>& hu, set<pt>& hd) {
	vector<pt> poly;
	hu.erase(hu.begin());
	for(pt p : hu) poly.push_back(p);
	hd.erase(hd.begin());
	for(pt p : hd) poly.push_back(-p);
	return poly;
}

/**
 * GENERAL POLYGONS
 */

//is pt p inside the (not necessarily convex) polygon given by poly
bool in_poly(pt p, vector<pt>& poly) {
	line l = {p, {INF, INF * PI}, 1};
	bool ans = false;
	pt lst = poly.back();
	for(pt q : poly) {
		line s = s2p(q, lst); lst = q;
		if(on_line(p, s)) return true; //change if border not included
		else if(!!(intsct(l, s) - INV)) ans = !ans;
	}
	return ans;
}

//area of polygon, vertices in order (cw or ccw)
ld area(vector<pt>& poly) {
	ld ans = 0;
	pt lst = poly.back();
	for(pt p : poly) ans += CRS(lst, p), lst = p;
	return abs(ans / 2);
}

//perimeter of polygon, vertices in order (cw or ccw)
ld perim(vector<pt>& poly) {
	ld ans = 0;
	pt lst = poly.back();
	for(pt p : poly) ans += abs(lst - p), lst = p;
	return ans;
}

//centroid of polygon, vertices in order (cw or ccw)
pt centroid(vector<pt>& poly) {
	ld area = 0;
	pt lst = poly.back(), ans = {0, 0};
	for(pt p : poly) {
		area += CRS(lst, p);
		ans += CRS(lst, p) * (lst + p) / (ld)3;
		lst = p;
	}
	return ans / area;
}

/**
 * CIRCLE FUNCTIONS
 */

//vector of intersection pts of two circs (up to 2) (if circles same, returns empty vector)
vector<pt> intsctCC(circ c1, circ c2) {
	ld d = abs(c1.C - c2.C);
	if(d > c1.R + c2.R || d < abs(c1.R - c2.R) || !d) return {};
	ld h = (d * d - c2.R * c2.R + c1.R * c1.R) / (2 * d);
	pt v = U(I * (c2.C - c1.C)) * sqrt(d * d - h * h);
	pt p = c1.C + U(c2.C - c1.C) * h;
	vector<pt> ans = {p + v};
	if(!!v) ans.push_back(p - v);
	return ans;
}

//vector of intersection pts of a line and a circ (up to 2)
vector<pt> intsctCL(circ c, line l) {
	if(dist_to(c.C, l) > c.R) return {};
	pt p = cl_pt_on_l(c.C, ml(l));
	pt v = U(l.D) * sqrt(c.R * c.R - norm(p - c.C));
	vector<pt> ans;
	if(on_line(p + v, l)) ans.push_back(p + v);
	if(on_line(p - v, l) && !!v) ans.push_back(p - v);
	return ans;
}

//external tangents of two circles (negate c2.R for internal tangents)
vector<line> circTangents(circ c1, circ c2) {
	pt d = c2.C - c1.C;
	ld dr = c1.R - c2.R, d2 = norm(d), h2 = d2 - dr * dr;
	if(!d2 || h2 < 0) return {};
	vector<line> ans;
	for(ld sg : {-1, 1}) {
		pt u = (d * dr + d * I * sqrt(h2) * sg) / d2;
		ans.push_back(s2p(c1.C + u * c1.R, c2.C + u * c2.R));
	}
	if(!h2) ans.pop_back();
	return ans;
}
