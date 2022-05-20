//Basic Template
#pragma GCC target("avx2")
#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl; 
typedef vector<ll> vl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define all(a) begin(a), end(a)
#define K first
#define V second

int main() {
    cin.tie(0)->sync_with_stdio(0);
}

//Build File
g++ --std=gnu++17 -Wall -Wshadow -Wno-sign-compare -fsanitize=address -fsanitize=undefined $1.cpp -o $1 2>&1 | more

//File IO
freopen("in.txt", "r", stdin);
freopen("out.txt", "w", stdout);

//PBDS
#include <bits/extc++.h>
using namespace __gnu_cxx;
using namespace __gnu_pbds;

typedef tree<ll, null_type, less<ll>,
rb_tree_tag, tree_order_statistics_node_update> set_t;
struct rhash {
    const ll seed = __builtin_ia32_rdtsc();
    ll operator()(ll x) const {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31) ^ seed;
    }
};
typedef gp_hash_table<ll, ll, rhash> table_t;

//Centroid Decomposition
vl tree[N];
ll sz[N], cPar[N], lvl[N]; //lvl is 1-indexed

ll getSize(ll i, ll p) {
    sz[i] = 1;
    for(ll j : tree[i])
        if(j - p && !lvl[j])
            sz[i] += getSize(j, i);
    return sz[i];
}

ll centroid(ll i, ll p, ll n) {
    for(ll j : tree[i])
        if(j - p && !lvl[j] && sz[j] > n / 2)
            return centroid(j, i, n);
    return i;
}

ll decomp(ll i, ll l) {
    ll cent = centroid(i, -1, getSize(i, -1));
    lvl[cent] = l;
    for(ll j : tree[cent]) if(!lvl[j])
        cPar[decomp(j, l + 1)] = cent;
    return cent;
}

//DFS Tree
vl graph[N], backIn[N], backOut[N], tree[N];
ll dep[N], par[N];

void dfs(ll i, ll p) {
    par[i] = p;
    dep[i] = dep[p] + 1;
    for(ll j : graph[i]) if(!dep[j]) {
        tree[i].push_back(j);
        dfs(j, i);
    } else if(dep[j] < dep[i] - 1) {
        backIn[j].push_back(i);
        backOut[i].push_back(j);
    }
}

//Dijkstra
vector<pl> graph[N];
ll n, d[N], p[N];

void dijk(int s) {
    fill_n(d, n + 1, LLONG_MAX);
    d[s] = p[s] = 0;
    priority_queue<pl, vector<pl>, greater<pl>> q;
    q.emplace(0, s);
    while(q.size()) {
        ll du = q.top().K, u = q.top().V;
        q.pop();
        if(du == d[u]) for(pl e : graph[u]) {
            ll v = e.K, dv = du + e.V;
            if(dv < d[v]) d[v] = dv, p[v] = u, q.emplace(dv, v);
        }
    }
}

//LCA
ll dep[N], par[N][L];
vl tree[N];

void dfs(ll i, ll p) {
    dep[i] = dep[p] + 1;
    par[i][0] = p;
    F(l, 1, L) par[i][l] = par[par[i][l - 1]][l - 1];
    for(ll j : tree[i]) if(j - p) dfs(j, i);
}

ll lca(ll a, ll b) {
    if(dep[a] < dep[b]) swap(a, b);
    for(ll l = L - 1; ~l; --l) if((dep[a] - dep[b]) >> l) a = par[a][l];
    if(a == b) return a;
    for(ll l = L - 1; ~l; --l) if(par[a][l] - par[b][l]) a = par[a][l], b = par[b][l];
    return par[a][0];
}

//Maxflow
#define S 1010

struct dinic {
    struct edge { ll b, cap, flow, flip; };
    vector<edge> g[S + 2];
    ll ans = 0, d[S + 2], ptr[S + 2];

    void add_edge(ll a, ll b, ll cap) {
        g[a].push_back({b, cap, 0, g[b].size()});
        g[b].push_back({a, 0, 0, g[a].size() - 1});
    }

    ll dfs(ll v, ll flow = LLONG_MAX) {
        if(v == S + 1 || !flow) { ans += flow; return flow; }
        while(++ptr[v] < g[v].size()) {
            edge &e = g[v][ptr[v]];
            if(d[e.b] != d[v] + 1) continue;
            if(ll p = dfs(e.b, min(flow, e.cap - e.flow))) {
                e.flow += p;
                g[e.b][e.flip].flow -= p;
                return p;
            }
        }
        return 0;
    }

    ll calc() {
        while(1) {
            vl q{S};
            memset(d, 0, sizeof d);
            ll i = -(d[S] = 1);
            while(++i < q.size() && !d[S + 1])
                for(auto e : g[q[i]])
                    if(!d[e.b] && e.flow < e.cap) {
                        q.push_back(e.b);
                        d[e.b] = d[q[i]] + 1;
                    }
            if(!d[S + 1]) return ans;
            memset(ptr, -1, sizeof ptr);
            while(dfs(S));
        }
    }
};

//SCCs
vl graph[N], rGraph[N], sccList[N], ord;
set<ll> sccGraph[N];
ll scc[N]; //sccs are 1-indexed
bool used[N];

void dfs1(ll i) {
    used[i] = true;
    for(ll j : graph[i]) if(!used[j]) dfs1(j);
    ord.push_back(i);
}

void dfs2(ll i, ll c) {
    scc[i] = c;
    sccList[c].push_back(i);
    for(ll j : rGraph[i]) if(!scc[j]) dfs2(j, c);
}

int main() {
    G(n) G(m)
    while(m--) {
        G(u) G(v)
        graph[u].push_back(v);
        rGraph[v].push_back(u);
    }
    F(i, 1, n + 1) if(!used[i]) dfs1(i);
    ll c = 1;
    reverse(ord.begin(), ord.end());
    for(ll i : ord) if(!scc[i]) dfs2(i, c++);
    F(i, 1, n + 1) for(ll j : graph[i]) if(scc[j] - scc[i]) sccGraph[scc[i]].insert(scc[j]);
}

//HLD
vl tree[N];
T val[N];
ll dep[N], idx[N], sz[N], top[N];

namespace sgtree {
    T id = 0, t[2 * N];
    T f(T a, T b) { return a + b; }

    void modify(ll i, T v) { //set value v at vertex i
        for(t[i = idx[i] + N] = v; i /= 2;) t[i] = f(t[2 * i], t[2 * i + 1]);
    }

    T query(ll i, ll a) { //fold f on first a ancestors of i
        ll r = idx[i] + 1 + N, l = r - a;
        T res = id;
        for(; l < r; l /= 2, r /= 2) {
            if(l & 1) res = f(res, t[l++]);
            if(r & 1) res = f(res, t[--r]);
        }
        return res;
    }
}

ll dfs1(ll i, ll p) {
    dep[i] = dep[p] + 1;
    sz[i] = 1;
    for(ll j : tree[i]) if(j - p)
        sz[i] += dfs1(j, i);
    sort(all(tree[i]), [](ll j, ll k) { return sz[j] > sz[k]; });
    return sz[i];
}

ll pos = 0;
void dfs2(ll i, ll p, ll t) {
    top[i] = t;
    idx[i] = pos++;
    sgtree::modify(i, val[i]);
    for(ll j : tree[i]) if(j - p)
        dfs2(j, i, t), t = i;
}

T query(ll a, ll b) {
    if(dep[a] - dep[b] == idx[a] - idx[b]) {
        if(dep[a] > dep[b]) swap(a, b);
        return sgtree::query(b, dep[b] - dep[a] + 1); //remove +1 if querying vals at edges (not vertices)
    }
    if(dep[top[a]] > dep[top[b]]) swap(a, b);
    return sgtree::f(query(a, top[b]), sgtree::query(b, dep[b] - dep[top[b]]));
}

int main() {
    G(n)
    F(i, 0, n - 1) {
        G(u) G(v)
        tree[u].push_back(v);
        tree[v].push_back(u);
    }
    dfs1(1, N - 1); //N - 1 is unused root vertex
    //if values initially on edges, fill val[] here
    dfs2(1, N - 1, N - 1);
}


//Segment Tree
namespace sgtree {
    typedef ll T;
    T id = 0, t[2 * N];

    T f(T a, T b) { return a + b; }

    void modify(ll p, T v) { //set value v at position p
        for(t[p += N] = v; p /= 2;) t[p] = f(t[2 * p], t[2 * p + 1]);
    }

    T query(ll l, ll r) { //query on interval [l, r)
        T resl = id, resr = id;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(l & 1) resl = f(resl, t[l++]);
            if(r & 1) resr = f(t[--r], resr);
        }
        return f(resl, resr);
    }
}

//Lazy Segment Tree
namespace lztree {
    typedef ll T;
    typedef ll U;

    T idT = 0, t[2 * N];
    U idU = 0, d[N];
    ll x = (fill_n(d, N, idU), 0);

    T f(T a, T b) { return a + b; }
    U g(U b, U a) { return a + b; }
    T h(U b, T a) { return a + b; }

    void calc(ll p) { t[p] = h(d[p], f(t[p * 2], t[p * 2 + 1])); }

    void apply(ll p, U v) {
        t[p] = h(v, t[p]);
        if(p < N) d[p] = g(v, d[p]);
    }

    void push(ll p) {
        p += N;
        FD(s, L, 0) {
            ll i = p >> s;
            if(d[i] != idU) {
                apply(i * 2, d[i]);
                apply(i * 2 + 1, d[i]);
                d[i] = idU;
            }
        }
    }

    void modify(ll p, T v) {
        push(p);
        t[p += N] = v;
        while(p > 1) calc(p /= 2);
    }

    void modify(ll l, ll r, U v) {
        push(l), push(r - 1);
        bool cl = false, cr = false;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(cl) calc(l - 1);
            if(cr) calc(r);
            if(l & 1) apply(l++, v), cl = true;
            if(r & 1) apply(--r, v), cr = true;
        }
        for(--l; r; l /= 2, r /= 2) {
            if(cl) calc(l);
            if(cr) calc(r);
        }
    }

    T query(ll l, ll r) {
        push(l), push(r - 1);
        T resl = idT, resr = idT;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(l & 1) resl = f(resl, t[l++]);
            if(r & 1) resr = f(t[--r], resr);
        }
        return f(resl, resr);
    }
}

//Mergesort Tree
namespace mstree {
    vl t[2 * N];

    ll f(ll i, ll k) {
        return (ll)(lower_bound(all(t[i]), k) - t[i].begin());
    }

    void build() { //call after setting t[i + N] for i in [0, n)
        for(ll i = N - 1; i; --i) merge(all(t[2 * i]), all(t[2 * i + 1]), back_inserter(t[i]));
    }

    ll query(ll l, ll r, ll k) {
        ll res = 0;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(l & 1) res += f(l++, k);
            if(r & 1) res += f(--r, k);
        }
        return res;
    }
}

//Mo's Algorithm
#define S 315

ll a[N], l[N], r[N], ans[N], curAns = 0;

void update(ll i, ll d) {
    //IMPLEMENT
}

int main() {
    G(n) G(m) vl q;
    F(i, 0, n) cin >> a[i];
    F(i, 0, m) cin >> l[i] >> r[i], q.push_back(i);
    sort(all(q), [](ll i, ll j) {
        if(l[i] / S - l[j] / S) return l[i] / S < l[j] / S;
        return l[i] / S % 2 ? r[i] > r[j] : r[i] < r[j];
    });
    ll cL = 0, cR = -1;
    for(ll i : q) {
        while(cL > l[i]) update(--cL, 1);
        while(cR < r[i]) update(++cR, 1);
        while(cL < l[i]) update(cL++, -1);
        while(cR > r[i]) update(cR--, -1);
        ans[i] = curAns;
    }
    F(i, 0, m) cout << ans[i] << '\n';
}

//Sparse Table
ll rmq[N][L];

ll f(ll a, ll b) { return min(a, b); }

ll query(ll l, ll r) { //half open interval [l, r)
    ll k = 63 - __builtin_clzll(r - l);
    return f(rmq[l][k], rmq[r - (1 << k)][k]);
}

int main() {
    G(n) F(i, 0, n) cin >> rmq[i][0];
    F(j, 1, L) F(i, 0, n) {
        ll i2 = i + (1 << (j - 1));
        if(i2 < n) rmq[i][j] = f(rmq[i][j - 1], rmq[i2][j - 1]);
        else rmq[i][j] = rmq[i][j - 1];
    }
}

//String Hashing
#define M 1000000321
#define OP(x, y) pl operator x (pl a, pl b) { return { a.K x b.K, (a.V y b.V) % M }; }
OP(+, +) OP(*, *) OP(-, + M -)
mt19937 gen(__builtin_ia32_rdtsc());
uniform_int_distribution<ll> dist(256, M - 1);

#define H(i, l) (h[(i) + (l)] - h[i] * p[l])
#define EQ(i, j, l) (H(i, l) == H(j, l))

string s;
pl p[N], h[N];
ll n, suff[N];

ll lcp(ll i, ll j, ll l, ll r) {
    if(l == r) return l;
    ll m = (l + r + 1) / 2;
    return EQ(i, j, m) ? lcp(i, j, m, r) : lcp(i, j, l, m - 1);
}

bool lexLess(ll i, ll lI, ll j, ll lJ) {
    if(EQ(i, j, min(lI, lJ))) return lI < lJ;
    ll m = lcp(i, j, 0, min(lI, lJ) - 1);
    return s[i + m] < s[j + m];
}

int main() {
    cin >> n >> s;
    p[0] = { 1, 1 }, p[1] = { dist(gen) | 1, dist(gen) };
    F(i, 1, s.size() + 1) p[i] = p[i - 1] * p[1], h[i] = h[i - 1] * p[1] + pl{s[i - 1], s[i - 1]};
    iota(suff, suff + n, 0);
    sort(suff, suff + n, [](ll i, ll j) { return lexLess(i, n - i, j, n - j); });
}

//Suffix Tree
map<char, ll> to[N], lk[N];
ll lf[N], rt[N], par[N], path[N];

#define att(a, b, c) to[par[a] = b][s[lf[a] = c]] = a;
void build(string s) {
	ll n = s.size(), z = 2;
	lf[1]--;
    for(ll i = n - 1; ~i; --i) {
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

//Convex Hull Trick
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
	seg s = *--upper_bound(all(hull), x, [](ll a, seg b) { return a < b.x; });
	return s.b + s.m * x;
}

//Gaussian Elimination
typedef vector<vector<ld>> mat;

ld elim(mat& a) { //a is n x m augmented matrix (m >= n), returns determinant
    ll n = a.size(), m = a[0].size();
    ld det = 1;
    F(i, 0, n) {
        ll bI = i;
        F(j, i, n) if(fabs(a[j][i]) > fabs(a[bI][i])) bI = j;
        swap(a[i], a[bI]);
        det *= i - bI ? -a[i][i] : a[i][i];
        if(!det) return 0;
        for(ll k = m - 1; k >= i; --k) a[i][k] /= a[i][i];
        F(j, 0, n) if(j - i) for(ll k = m - 1; k >= i; --k) a[j][k] -= a[j][i] * a[i][k];
    }
    return det;
}

//LIS
vl lis;
ll n, a[N];

int main() {
    F(i, 0, n) {
        if(lis.empty() || a[i] >= lis.back()) lis.push_back(a[i]); //change to > for strictly increasing
        else *upper_bound(all(lis), a[i]) = a[i]; //change to lower_bound for strictly increasing
    }
    cout << lis.size() << '\n';
}

//Number Theory Stuff
ll pw(ll a, ll p) { return p ? pw(a * a % M, p / 2) * (p & 1 ? a : 1) % M : 1; }

ll inv(ll a, ll b = M) { return 1 < a ? b - inv(b % a, a) * b / a : 1; } //inv a mod b

ll phi(ll n) {
    ll ans = n;
    while(n > 1) {
        ll p = prime[n];
        while(!(n % p)) n /= p;
        ans = ans / p * (p - 1);
    }
    return ans;
}

//NTT
ll root = 62;
vl rt(2, 1);

void ntt(vl &a) {
	ll n = a.size();
	vl rev(n);
	F(i, 0, n) if(i < (rev[i] = (rev[i / 2] | (i & 1 ? n : 0)) / 2)) swap(a[i], a[rev[i]]);
	for(ll k = 1; k < n; k *= 2)
		for(ll i = 0; i < n; i += 2 * k) F(j, 0, k) {
			ll z = rt[j + k] * a[i + j + k] % M, &ai = a[i + j];
			a[i + j + k] = (ai - z + M) % M;
			ai = (ai + z) % M;
		}
}

vl mult(vl a, vl b) {
	ll n = a.size() + b.size();
    while(__builtin_popcountll(n) > 1) n++;
    a.resize(n), b.resize(n);
    vl out(n); ll in = inv(n);
	ntt(a), ntt(b);
	F(i, 0, n) out[-i & (n - 1)] = a[i] * b[i] % M * in % M;
	ntt(out);
	return out;
}

int main() {
    F(s, 2, L) {
		ll z[] = {1, pw(root, M >> s)};
		F(i, 1 << (s - 1), 1 << s) rt.push_back(rt[i / 2] * z[i & 1] % M);
	}
}

//FFT
vector<cd> rt(2, 1);

void fft(vector<cd>& a) {
    ll n = a.size();
    vl rev(n);
    F(i, 0, n) if(i < (rev[i] = (rev[i / 2] | (i & 1 ? n : 0)) / 2)) swap(a[i], a[rev[i]]);
    for(ll k = 1; k < n; k *= 2)
        for(ll i = 0; i < n; i += 2 * k) F(j, 0, k) {
            cd z = rt[j + k] * a[i + j + k];
            a[i + j + k] = a[i + j] - z;
            a[i + j] += z;
        }
}

vector<ld> mult(vector<ld> a, vector<ld> b) {
    ll n = a.size() + b.size();
    while(__builtin_popcountll(n) > 1) n++;
    vector<cd> in(n), out(n);
    a.resize(n), b.resize(n);
    F(i, 0, n) in[i] = {a[i], b[i]};
    fft(in);
    for(cd& x : in) x *= x;
    F(i, 0, n) out[i] = in[-i & (n - 1)] - conj(in[i]);
    fft(out);
    vector<ld> res(n);
    F(i, 0, n) res[i] = out[i].imag() / 4 / n;
    return res;
}

int main() {
    for(ll k = 2; k < (1 << L); k *= 2) {
        cd x = polar(1.l, acos(-1.l) / k);
        F(i, k, 2 * k) rt.push_back(rt[i / 2] * (i & 1 ? x : 1));
    }
}

//Geometry Header
typedef complex<ld> pt;
struct line {
	pt P, D; bool S = false;
	line(pt p, pt q, bool b = false) : P(p), D(q - p), S(b) {}
	line(pt p, ld th) : P(p), D(polar((ld)1, th)) {}
};
struct circ { pt C; ld R; };

#define X real()
#define Y imag()
#define CRS(a, b) (conj(a) * (b)).Y
#define DOT(a, b) (conj(a) * (b)).X
#define U(p) ((p) / abs(p))
#define Z(x) (abs(x) < EPS)

ld PI = acosl(-1), INF = 1e20, EPS = 1e-12;
pt I = {0, 1};

namespace std {
	bool operator<(pt a, pt b) { return Z(a.X - b.X) ? a.Y < b.Y : a.X < b.X; }
}

//General Geometry
bool parallel(pt d1, pt d2) { return Z(d1) || Z(d2) || Z(CRS(U(d1), U(d2))); }

bool above_line(pt p, line l) { return CRS(p - l.P, l.D) > 0; }

bool on_line(pt p, line l) { return parallel(l.P - p, l.D) && (!l.S || DOT(l.P - p, l.P + l.D - p) <= EPS); }

ll intsct(line l1, line l2, pt& p) {
	if(parallel(l1.D, l2.D)) return on_line(l1.P, l2) ? 2 : 0;
	pt q = l1.P + l1.D * CRS(l2.D, l2.P - l1.P) / CRS(l2.D, l1.D);
	if(on_line(q, l1) && on_line(q, l2)) { p = q; return 1; }
	return 0;
}

pt cl_pt_on_l(pt p, line l) {
	pt q = l.P + DOT(U(l.D), p - l.P) * U(l.D);
	if(on_line(q, l)) return q;
	return abs(p - l.P) < abs(p - l.P - l.D) ? l.P : l.P + l.D;
}

ld dist_to(pt p, line l) { return abs(p - cl_pt_on_l(p, l)); }

pt refl_pt(pt p, line l) { return conj((p - l.P) / U(l.D)) * U(l.D) + l.P; }

line reflect_line(line r, line l) {
	pt p; if(intsct(r, l, p) - 1) return r;
	return line(p, p + INF * (p - refl_pt(r.P, l)), 1);
}

//Triangle Centers
line alt(pt p, line l) { l.S = 0; return line(p, cl_pt_on_l(p, l)); }

line ang_bis(pt a, pt b, pt c) { return line(b, b + INF * (U(a - b) + U(c - b)), 1); }

line perp_bis(line l) { return line(l.P + l.D / (ld)2, arg(l.D * I)); }

pt orthocent(pt a, pt b, pt c) { pt p; intsct(alt(a, line(b, c)), alt(b, line(a, c)), p); return p; }

circ incirc(pt a, pt b, pt c) {
	pt cent; intsct(ang_bis(a, b, c), ang_bis(b, a, c), cent);
	return {cent, dist_to(cent, line(a, b))};
}

circ circumcirc(pt a, pt b, pt c) {
	pt cent; intsct(perp_bis(line(a, b, 1)), perp_bis(line(a, c, 1)), cent);
	return {cent, abs(cent - a)};
}

//Convex Hull
bool in_hull(pt p, pair<vector<pt>, vector<pt>>& h) {
	if(Z(p - *h.first.begin()) || Z(p - *h.second.begin())) return false; //change to true if border counts as inside
	if(p < *h.first.begin() || *h.second.begin() < p) return false;
	auto u = upper_bound(all(h.first), p);
	auto d = lower_bound(h.second.rbegin(), h.second.rend(), p);
	return CRS(*u - p, *(u - 1) - p) > EPS && CRS(*(d - 1) - p, *d - p) > EPS; //change to > -EPS if border counts as "inside"
}

void do_hull(vector<pt>& pts, vector<pt>& h) {
	for(pt p : pts) {
		while(h.size() > 1 && CRS(h.back() - p, h[h.size() - 2] - p) <= EPS)
			h.pop_back();
		h.push_back(p);
	}
}

pair<vector<pt>, vector<pt>> get_hull(vector<pt>& pts) {
	vector<pt> hu, hd;
	sort(all(pts)), do_hull(pts, hu);
	reverse(all(pts)), do_hull(pts, hd);
	return {hu, hd};
}

vector<pt> full_hull(vector<pt>& pts) {
	auto h = get_hull(pts);
	h.first.pop_back(), h.second.pop_back();
	for(pt p : h.second) h.first.push_back(p);
	return h.first;
}

//Polygon functions
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

ld area(vector<pt>& poly) {
	ld ans = 0;
	pt lst = poly.back();
	for(pt p : poly) ans += CRS(lst, p), lst = p;
	return abs(ans / 2);
}

ld perim(vector<pt>& poly) {
	ld ans = 0;
	pt lst = poly.back();
	for(pt p : poly) ans += abs(lst - p), lst = p;
	return ans;
}

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

//Circle Functions
vector<pt> intsctCC(circ c1, circ c2) {
    pt d = c2.C - c1.C;
    ld d2 = norm(d);
    if(Z(d)) return {}; //concentric
    ld pd = (d2 + c1.R * c1.R - c2.R * c2.R) / 2;
    ld h2 = c1.R * c1.R - pd * pd / d2;
    if(h2 < 0) return {};
    pt p = c1.C + d * pd / d2, h = d * I * sqrtl(h2 / d2);
    if(Z(h)) return {p};
    return {p - h, p + h};
}

vector<pt> intsctCL(circ c, line l) {
    vector<pt> v, ans;
    if(Z(dist_to(c.C, line(l.P, l.P + l.D, 0)))) v = {c.C + c.R * U(l.D), c.C - c.R * U(l.D)};
    else v = intsctCC(c, circ{refl_pt(c.C, l), c.R});
	for(pt p : v) if(on_line(p, l)) ans.push_back(p);
    return ans;
}

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
