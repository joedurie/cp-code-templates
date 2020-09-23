//Maxflow
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define N 100010

struct max_flow {
    const static ll MAXN = 2000; // max_num_vertices
    struct edge { ll a, b, cap, flow; };

    ll n, s, t, d[MAXN] = {}, ptr[MAXN] = {}, q[MAXN] = {};
    vector<edge> e;
    vector<ll> g[MAXN];

    max_flow(ll n0, ll s0, ll t0) : n(n0), s(s0), t(t0) {}

    void add_edge(ll a, ll b, ll cap) {
        edge e1 = { a, b, cap, 0 }, e2 = { b, a, 0, 0 };
        g[a].push_back(e.size());
        e.push_back(e1);
        g[b].push_back(e.size());
        e.push_back(e2);
    }

    bool bfs() {
        ll qh = 0, qt = 0;
        q[qt++] = s;
        fill_n(d, n, -1);
        d[s] = 0;
        while(qh < qt && d[t] == -1) {
            ll v = q[qh++];
            F(i, 0, g[v].size()) {
                ll id = g[v][i],
                to = e[id].b;
                if(d[to] == -1 && e[id].flow < e[id].cap) {
                    q[qt++] = to;
                    d[to] = d[v] + 1;
                }
            }
        }
        return ~d[t];
    }

    ll dfs(ll v, ll flow) {
        if(!flow) return 0;
        if(v == t) return flow;
        for(; ptr[v] < (ll)g[v].size(); ++ptr[v]) {
            ll id = g[v][ptr[v]], to = e[id].b;
            if(d[to] != d[v] + 1) continue;
            ll pushed = dfs(to, min(flow, e[id].cap - e[id].flow));
            if(pushed) {
                e[id].flow += pushed;
                e[id ^ 1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }

    ll dinic() {
        ll flow = 0;
        while(bfs()) {
            fill_n(ptr, n, 0);
            while(ll pushed = dfs(s, 1000000000)) flow += pushed;
        }
        return flow;
    }
};
