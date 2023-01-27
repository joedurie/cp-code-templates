//Cuckoo Hash Map
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

struct rhash {
    const ll seed = __builtin_ia32_rdtsc();
    ll operator()(ll x) const {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31) ^ seed;
    }
};

template <typename T, typename U, ll C> struct cmap {
    rhash hsh; pair<T, U> tbl[1 << C], empt = {-1, 0};

    #define I1(h) ((h) & ((1 << C) - 1))
    #define XR(x) I1(x >> C)

    void set(T t, U u) {
        ll h = hsh(t), i = I1(h), j = i ^ XR(h);
        if(tbl[i].K == t) tbl[i].V = u;
        else if(tbl[j].K == t) tbl[j].V = u;
        else while(1) {
            pair<T, U> v = tbl[i];
            tbl[i] = {t, u};
            if(v == empt) return;
            i ^= XR(hsh(t = v.K)), u = v.V;
        }
    }

    bool count(T t) {
        ll h = hsh(t), i = I1(h);
        return tbl[i].K == t || tbl[i ^ XR(h)].K == t;
    }

    U operator[](T t) {
        ll h = hsh(t), i = I1(h);
        if(tbl[i].K == t) return tbl[i].V;
        if(tbl[i ^= XR(h)].K == t) return tbl[i].V;
        return empt.V;
    }

    void erase(T t) {
        ll h = hsh(t), i = I1(h);
        if(tbl[i].K == t) tbl[i] = empt;
        else if(tbl[i ^= XR(h)].K == t) tbl[i] = empt;
    }

    void clear() { fill_n(tbl, 1 << C, empt); }
};
