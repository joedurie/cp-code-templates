//String Hashing
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define K first
#define V second
#define M 1000000321
#define OP(x, y) pl operator x (pl a, pl b) { return { a.K x b.K, (a.V y b.V) % M }; }
OP(+, +) OP(*, *) OP(-, + M -)
mt19937 gen(__builtin_ia32_rdtsc());
uniform_int_distribution<ll> dist(256, M - 1);

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define H(i, l) (h[(i) + (l)] - h[i] * p[l])
#define EQ(i, j, l) (H(i, l) == H(j, l))
#define N 100010

string s;
pl p[N], h[N];
ll n, suff[N];

ll lcp(ll i, ll j, ll l, ll r) {
    if(l == r) return l;
    ll m = (l + r + 1) / 2;
    return EQ(i, j, m) ? lcp(i, j, m, r) : lcp(i, j, l, m - 1);
}

bool lexLess(ll i, ll lI, ll j, ll lJ) {
    if(EQ(i, j, min(lI, lJ))) return lI < lJ;
    ll m = lcp(i, j, 0, min(lI, lJ) - 1);
    return s[i + m] < s[j + m];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> s;
    p[0] = { 1, 1 }, p[1] = { dist(gen) | 1, dist(gen) };
    F(i, 1, s.size() + 1) p[i] = p[i - 1] * p[1], h[i] = h[i - 1] * p[1] + make_pair(s[i - 1], s[i - 1]);
    iota(suff, suff + n, 0);
    sort(suff, suff + n, [](ll i, ll j) { return lexLess(i, n - i, j, n - j); });
}
