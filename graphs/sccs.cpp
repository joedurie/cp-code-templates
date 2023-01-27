//Strongly Connected Components
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
    ios_base::sync_with_stdio(0);
    cin.tie(0);
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
