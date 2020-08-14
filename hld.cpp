//Heavy-Light Decomposition
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;
typedef ll T;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); i++)
#define UPD(v, k) stree.modify(idx[v], k);
#define K first
#define V second
#define N 100010

struct segtree { // simplified for associative operations
    T id = 0, t[2 * N];
    T f(T a, T b) { return max(a, b); }

    void modify(ll p, T value) {  // set value at position p
        for (t[p += N] = value; p /= 2;) t[p] = f(t[2 * p], t[2 * p + 1]);
    }

    T query(ll l, ll r) { // fold f on interval [l, r)
        T res = id;
        for (l += N, r += N; l < r; l /= 2, r /= 2) {
            if (l & 1) res = f(res, t[l++]);
            if (r & 1) res = f(res, t[--r]);
        }
        return res;
    }
};

vector<pair<ll, T>> tree[N];
ll sz[N], dep[N], idx[N], par[N], top[N];
struct segtree stree;

ll dfs1(ll i, ll p, ll d) {
    dep[i] = d;
    sz[i] = 1;
    par[i] = p;
    for(auto a : tree[i]) if(a.K - p)
        sz[i] += dfs1(a.K, i, d + 1);
    return sz[i];
}

ll pos = 0;
void dfs2(ll i, T w, ll p, ll t) {
    stree.modify(pos, w);
    idx[i] = pos++;
    top[i] = t;
    for(auto a : tree[i]) 
        if(a.K - p && sz[a.K] > (sz[i] - 1) / 2)
            dfs2(a.K, a.V, t);
    for(auto a : tree[i])
        if(a.K - p && !idx[a.K])
            dfs2(a.K, a.V, a.K);
}

ll query(ll a, ll b) { // only works for associative operations - non-associative very messy
    if(top[a] == top[b]) {
        if(dep[a] > dep[b]) swap(a, b);
        return stree.query(idx[a] + 1, idx[b] + 1); // remove the + 1 from idx[a] if querying values on vertices
    }
    if(dep[top[a]] > dep[top[b]]) swap(a, b);
    T val = stree.query(top[b] == b ? idx[b] : idx[top[b]] + 1, idx[b] + 1);
    return stree.f(val, query(a, top[b] == b ? par[b] : top[b]));
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    G(n)
    F(i, 0, n - 1) {
        G(u) G(v) G(w)
        tree[u].emplace_back(v, w);
        tree[v].emplace_back(u, w);
    }
    dfs1(1, -1, 1);
    dfs2(1, -1, 1);
    G(q) while(q--) {
        // query(a, b) or UPD(v, k) as necessary
    }
}
