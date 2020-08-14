//Segment Tree
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

struct segtree {
    typedef ll T;
    T id = 0, t[2 * N];
    ll n;  // array size (use N for single tc)
    T f(T a, T b) { return a + b; }

    void modify(ll p, T value) {  // set value at position p
        for (t[p += n] = value; p /= 2;) t[p] = f(t[2 * p], t[2 * p + 1]);
    }

    T query(ll l, ll r) { // query on interval [l, r)
        T resl = id, resr = id;
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l & 1) resl = f(resl, t[l++]);
            if (r & 1) resr = f(t[--r], resr);
        }
        return f(resl, resr);
    }
};
