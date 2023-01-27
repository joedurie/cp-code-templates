//Merge Sort Tree
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
#define FD(i, r, l) for(ll i = r; i > (l); --i)
#define A(a) (a).begin(), (a).end()
#define N 100010

namespace mstree {
    vl t[2 * N];

    ll f(ll i, ll k) { //num elements < k in t[i]
        return (ll)(lower_bound(A(t[i]), k) - t[i].begin());
    }

    void build() { //call after setting t[i + N] for i in [0, n)
        FD(i, N - 1, 0) merge(A(t[2 * i]), A(t[2 * i + 1]), back_inserter(t[i]));
    }

    ll query(ll l, ll r, ll k) { //num elements < k in interval [l, r)
        ll res = 0;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(l & 1) res += f(l++, k);
            if(r & 1) res += f(--r, k);
        }
        return res;
    }
}
