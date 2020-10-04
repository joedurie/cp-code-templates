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
#define A(a) (a).begin(), (a).end()
#define N 100010

vector<ll> tree[N];
T val[N];
ll dep[N], idx[N], sz[N], top[N];

struct segtree { //modified for HLD operations
    T id = 0, t[2 * N];
    T f(T a, T b) { return max(a, b); }

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
};

struct segtree st;

ll dfs1(ll i, ll p) {
    dep[i] = dep[p] + 1;
    sz[i] = 1;
    for(ll j : tree[i]) if(j - p)
        sz[i] += dfs1(j, i);
    sort(A(tree[i]), [](ll j, ll k) { return sz[j] > sz[k]; });
    return sz[i];
}

ll pos = 0;
void dfs2(ll i, ll p, ll t) {
    top[i] = t;
    idx[i] = pos++;
    st.modify(i, val[i]);
    for(ll j : tree[i]) if(j - p)
        dfs2(j, i, t), t = i;
}

T query(ll a, ll b) { //only works for commutative operations - others very messy
    if(dep[a] - dep[b] == idx[a] - idx[b]) {
        if(dep[a] > dep[b]) swap(a, b);
        return st.query(b, dep[b] - dep[a] + 1); //remove +1 if querying vals at edges (not vertices)
    }
    if(dep[top[a]] > dep[top[b]]) swap(a, b);
    return st.f(query(a, top[b]), st.query(b, dep[b] - dep[top[b]]));
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
    dfs1(1, N - 1); //N - 1 is unused root vertex
    //if values initially on edges, fill val[] here
    dfs2(1, N - 1, N - 1);
    G(q) while(q--) {
        //query(a, b) or st.modify(a, v) as necessary
    }
}
