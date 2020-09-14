//DFS Tree
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); i++)
#define N 100010

vector<ll> graph[N], backIn[N], backOut[N], tree[N];
ll dep[N], par[N]; //make depth of root 1

ll dfs(ll i, ll p) {
    par[i] = p;
    dep[i] = dep[p] + 1;
    for(ll j : graph[i]) if(j - p) {
        if(!dep[j]) {
            tree[i].push_back(j);
            dfs(j, i);
        } else if(dep[j] < dep[i]) {
            backIn[j].push_back(i);
            backOut[i].push_back(j);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    G(n) G(m)
    while(m--) {
        G(u) G(v)
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    dfs(1, 1);
}
