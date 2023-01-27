//Maxflow
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define N 100010
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
