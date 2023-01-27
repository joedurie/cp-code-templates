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
#define FD(i, r, l) for(ll i = r - 1; i > (l); --i)
#define N 100010
#define L 20

namespace lztree {
    typedef ll T;
    typedef ll U;

    T idT = 0, t[2 * N];
    U idU = 0, d[N];
    ll x = (fill_n(d, N, idU), 0);

    T f(T a, T b) { return a + b; }
    U g(U b, U a) { return a + b; }
    T h(U b, T a) { return a + b; }

    void calc(ll p) { t[p] = h(d[p], f(t[p * 2], t[p * 2 + 1])); }

    void apply(ll p, U v) {
        t[p] = h(v, t[p]);
        if(p < N) d[p] = g(v, d[p]);
    }

    void push(ll p) {
        p += N;
        FD(s, L, 0) {
            ll i = p >> s;
            if(d[i] != idU) {
                apply(i * 2, d[i]);
                apply(i * 2 + 1, d[i]);
                d[i] = idU;
            }
        }
    }

    void modify(ll p, T v) {
        push(p);
        t[p += N] = v;
        while(p > 1) calc(p /= 2);
    }

    void modify(ll l, ll r, U v) {
        push(l), push(r - 1);
        bool cl = false, cr = false;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(cl) calc(l - 1);
            if(cr) calc(r);
            if(l & 1) apply(l++, v), cl = true;
            if(r & 1) apply(--r, v), cr = true;
        }
        for(--l; r; l /= 2, r /= 2) {
            if(cl) calc(l);
            if(cr) calc(r);
        }
    }

    T query(ll l, ll r) {
        push(l), push(r - 1);
        T resl = idT, resr = idT;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(l & 1) resl = f(resl, t[l++]);
            if(r & 1) resr = f(t[--r], resr);
        }
        return f(resl, resr);
    }
}
