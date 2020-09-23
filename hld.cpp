//Heavy-Light Decomposition
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;
typedef ll T;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define UPD(v, k) stree.modify(idx[v], k);
#define N 100010

vector<ll> tree[N];
T val[N];
ll dep[N], idx[N], heavy[N], top[N];

struct segtree { // simplified for commutative operations
    T id = 0, t[2 * N];
    T f(T a, T b) { return max(a, b); }

    void modify(ll p, T value) {  // set value at position p
        for (t[p += N] = value; p /= 2;) t[p] = f(t[2 * p], t[2 * p + 1]);
    }

    T query(ll i, ll a) { // fold f on first a ancestors of i
        ll l = idx[i] - a + 1, r = idx[i] + 1;
        T res = id;
        for (l += N, r += N; l < r; l /= 2, r /= 2) {
            if (l & 1) res = f(res, t[l++]);
            if (r & 1) res = f(res, t[--r]);
        }
        return res;
    }
};

struct segtree stree;

ll dfs1(ll i, ll p) {
    dep[i] = dep[p] + 1;
    heavy[i] = -1;
    ll sz = 1, mx = 0;
    for(ll j : tree[i]) if(j - p) {
        ll k = dfs1(j, i);
        if(k > mx) mx = k, heavy[i] = j;
        sz += k;
    }
    return sz;
}

ll pos = 0;
void dfs2(ll i, ll p, ll t) {
    stree.modify(pos, val[i]);
    idx[i] = pos++;
    top[i] = t == i ? p : t;
    if(~heavy[i]) dfs2(heavy[i], i, t);
    for(ll j : tree[i])
        if(j - p && j - heavy[i])
            dfs2(j, i, j);
}

ll query(ll a, ll b) { // only works for commutative operations - others very messy
    if(idx[a] - idx[b] == dep[a] - dep[b]) {
        if(dep[a] > dep[b]) swap(a, b);
        return stree.query(b, dep[b] - dep[a] + 1); //remove +1 if querying vals at edges (not vertices)
    }
    if(dep[top[a]] > dep[top[b]] || b == 1) swap(a, b);
    T v = stree.query(b, dep[b] - dep[top[b]]);
    return stree.f(v, query(a, top[b]));
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    G(n)
    F(i, 0, n - 1) {
        G(u) G(v)
        tree[u].push_back(v);
        tree[v].push_back(u);
    }
    dfs1(1, 1);
    //if values initially on edges, fill val[] here
    dfs2(1, 1, 1);
    G(q) while(q--) {
        // query(a, b) or UPD(v, k) as necessary
    }
}
