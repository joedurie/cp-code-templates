//Geometry
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef complex<ld> pt;
struct line {
	pt P, D; bool S = false;
	line(pt p, pt q, bool b = false) : P(p), D(q - p), S(b) {}
	line(pt p, ld th) : P(p), D(polar((ld)1, th)) {}
};
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
#define U(p) ((p) / abs(p)) //unit vector in direction of p (don't use if Z(p) == true)
#define Z(x) (abs(x) < EPS)
#define A(a) (a).begin(), (a).end() //shortens sort(), upper_bound(), etc. for vectors

//constants (INF and EPS may need to be modified)
ld PI = acosl(-1), INF = 1e20, EPS = 1e-12;
pt I = {0, 1};

/**
 * GENERAL GEOMETRY
 */

//true if d1 and d2 parallel (zero vectors considered parallel to everything)
bool parallel(pt d1, pt d2) { return Z(d1) || Z(d2) || Z(CRS(U(d1), U(d2))); }

//"above" here means if l & p are rotated such that l.D points in the +x direction, then p is above l
bool above_line(pt p, line l) { return CRS(p - l.P, l.D) > 0; }

//true if p is on line l
bool on_line(pt p, line l) { return parallel(l.P - p, l.D) && (!l.S || DOT(l.P - p, l.P + l.D - p) <= EPS); }

//returns 0 for no intersection, 2 for infinite intersections, 1 otherwise. p holds intersection pt
ll intsct(line l1, line l2, pt& p) {
	if(parallel(l1.D, l2.D)) //note that two parallel segments sharing one endpoint are considered to have infinite intersections here
		return 2 * (on_line(l1.P, l2) || on_line(l1.P + l1.D, l2) || on_line(l2.P, l1) || on_line(l2.P + l2.D, l1));
	pt q = l1.P + l1.D * CRS(l2.D, l2.P - l1.P) / CRS(l2.D, l1.D);
	if(on_line(q, l1) && on_line(q, l2)) { p = q; return 1; }
	return 0;
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
pt refl_pt(pt p, line l) { return conj((p - l.P) / U(l.D)) * U(l.D) + l.P; }

//ray r reflected off l (if no intersection, returns original ray)
line reflect_line(line r, line l) {
	pt p; if(intsct(r, l, p) - 1) return r;
	return line(p, p + INF * (p - refl_pt(r.P, l)), 1);
}

/**
 * TRIANGLE CENTERS
 */

//altitude from p to l
line alt(pt p, line l) { l.S = 0; return line(p, cl_pt_on_l(p, l)); }

//angle bisector of angle abc
line ang_bis(pt a, pt b, pt c) { return line(b, b + INF * (U(a - b) + U(c - b)), 1); }

//perpendicular bisector of l (assumes l.S == 1)
line perp_bis(line l) { return line(l.P + l.D / (ld)2, arg(l.D * I)); }

//orthocenter of triangle abc
pt orthocent(pt a, pt b, pt c) { pt p; intsct(alt(a, line(b, c)), alt(b, line(a, c)), p); return p; }

//incircle of triangle abc
circ incirc(pt a, pt b, pt c) {
	pt cent; intsct(ang_bis(a, b, c), ang_bis(b, a, c), cent);
	return {cent, dist_to(cent, line(a, b))};
}

//circumcircle of triangle abc
circ circumcirc(pt a, pt b, pt c) {
	pt cent; intsct(perp_bis(line(a, b, 1)), perp_bis(line(a, c, 1)), cent);
	return {cent, abs(cent - a)};
}

/**
 * GENERAL POLYGONS
 */

//is pt p inside the (not necessarily convex) polygon given by poly
bool in_poly(pt p, vector<pt>& poly) {
	line l = line(p, {INF, INF * PI}, 1);
	bool ans = false;
	pt lst = poly.back(), tmp;
	for(pt q : poly) {
		line s = line(q, lst, 1); lst = q;
		if(on_line(p, s)) return false; //change if border not included
		else if(intsct(l, s, tmp)) ans = !ans;
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
 * CIRCLES
 */

//vector of intersection pts of two circs (up to 2) (if circles same, returns empty vector)
vector<pt> intsctCC(circ c1, circ c2) {
	if(c1.R < c2.R) swap(c1, c2);
	pt d = c2.C - c1.C;
	if(Z(abs(d) - c1.R - c2.R)) return {c1.C + polar(c1.R, arg(c2.C - c1.C))};
	if(!Z(d) && Z(abs(d) - c1.R + c2.R)) return {c1.C + c1.R * U(d)}; 
	if(abs(d) >= c1.R + c2.R - EPS || abs(d) <= c2.R - c1.R + EPS) return {};
	ld th = acosl((c1.R * c1.R + norm(d) - c2.R * c2.R) / (2 * c1.R * abs(d)));
	return {c1.C + polar(c1.R, arg(d) + th), c1.C + polar(c1.R, arg(d) - th)};
}

//vector of intersection pts of a line and a circ (up to 2)
vector<pt> intsctCL(circ c, line l) {
	vector<pt> v, ans;
	if(Z(dist_to(c.C, line(l.P, l.P + l.D, 0)))) v = {c.C + c.R * U(l.D), c.C - c.R * U(l.D)};
	else v = intsctCC(c, circ{refl_pt(c.C, l), c.R});
	for(pt p : v) if(on_line(p, l)) ans.push_back(p);
	return ans;
}

//external tangents of two circles (negate c2.R for internal tangents)
vector<line> circTangents(circ c1, circ c2) {
	pt d = c2.C - c1.C;
	ld dr = c1.R - c2.R, d2 = norm(d), h2 = d2 - dr * dr;
	if(Z(d2) || h2 < 0) return {};
	vector<line> ans;
	for(ld sg : {-1, 1}) {
		pt u = (d * dr + d * I * sqrt(h2) * sg) / d2;
		ans.push_back(line(c1.C + u * c1.R, c2.C + u * c2.R, 1));
	}
	if(Z(h2)) ans.pop_back();
	return ans;
}
