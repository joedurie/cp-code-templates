//Fast Fourier Transform
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef complex<ld> cd;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define N 100010
#define L 20

vector<cd> rt(2, 1);

void fft(vector<cd>& a) {
    ll n = a.size(); vl rev(n);
    F(i, 0, n) if(i < (rev[i] = (rev[i / 2] | (i & 1 ? n : 0)) / 2)) swap(a[i], a[rev[i]]);
    for(ll k = 1; k < n; k *= 2)
        for(ll i = 0; i < n; i += 2 * k) F(j, 0, k) {
            cd z = rt[j + k] * a[i + j + k];
            a[i + j + k] = a[i + j] - z;
            a[i + j] += z;
        }
}

vector<ld> mult(vector<ld> a, vector<ld> b) {
    ll n = a.size() + b.size();
    while(__builtin_popcountll(n) > 1) n++;
    vector<cd> in(n), out(n);
    a.resize(n), b.resize(n);
    F(i, 0, n) in[i] = {a[i], b[i]};
    fft(in);
    for(cd& x : in) x *= x;
    F(i, 0, n) out[i] = in[-i & (n - 1)] - conj(in[i]);
    fft(out);
    vector<ld> res(n);
    F(i, 0, n) res[i] = out[i].imag() / 4 / n;
    return res;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    for(ll k = 2; k < (1 << L); k *= 2) {
        cd x = polar(1.l, acos(-1.l) / k);
        F(i, k, 2 * k) rt.push_back(rt[i / 2] * (i & 1 ? x : 1));
    }
}
