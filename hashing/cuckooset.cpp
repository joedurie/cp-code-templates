//Cuckoo Hash Set
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

template <typename T, ll C> struct cset {
    rhash hsh; T tbl[1 << C], empt = 0;
    const ll msk = (1 << C) - 1;

    #define I1(h) ((h) & msk)
    #define XR(x) I1(x >> C)

    void insert(T t) {
        ll h = hsh(t), i = I1(h);
        if(tbl[i] != t && tbl[i ^ XR(h)] != t) while(1) {
            T t2 = tbl[i];
            tbl[i] = t;
            if(t2 == empt) return;
            i ^= XR(hsh(t = t2));
        }
    }

    bool count(T t) {
        ll h = hsh(t), i = I1(h);
        return tbl[i] == t || tbl[i ^ XR(h)] == t;
    }

    void erase(T t) {
        ll h = hsh(t), i = I1(h);
        if(tbl[i] == t) tbl[i] = empt;
        else if(tbl[i ^= XR(h)] == t) tbl[i] = empt;
    }

    void clear() { fill_n(tbl, 1 << C, empt); }
};
