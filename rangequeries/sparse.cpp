//Sparse Table
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
#define L 20

ll bt[N], rmq[N][L];

ll f(ll a, ll b) { return min(a, b); } //must be idempotent

ll query(ll l, ll r) { //half open interval [l, r)
    return f(rmq[l][bt[r - l]], rmq[r - (1 << bt[r - l])][bt[r - l]]);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    F(j, 0, L) F(i, (1 << j), min(N, 1 << (j + 1))) bt[i] = j;
    G(n) F(i, 0, n) cin >> rmq[i][0];
    F(j, 1, L) F(i, 0, n) {
        ll i2 = i + (1 << (j - 1));
        if(i2 < n) rmq[i][j] = f(rmq[i][j - 1], rmq[i2][j - 1]);
        else rmq[i][j] = rmq[i][j - 1];
    }
}
