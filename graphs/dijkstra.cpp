//ICPC Template
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
#define K first
#define V second
#define N 100010

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

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n;
    G(m) while(m--) {
        G(u) G(v) G(w)
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }
    dijk(1);
}
