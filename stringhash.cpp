//String Hashing
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef pair<unsigned ll, ll> hsh;

#define K first
#define V second
#define M 1000000321
#define OP(x, y) constexpr hsh operator x (const hsh a, const hsh b) { return { a.K x b.K, (a.V y b.V) % M }; }
OP(+, +) OP(*, *) OP(-, + M -)
mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<ll> dist(256, M - 1);

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < r; i++)
#define EQ(i, j, l) ((h[i + l] - h[i]) * p[j] == (h[j + l] - h[j]) * p[i]) //does s[i, i + l) == s[j, j + l)
#define PERIODIC(l, r, k) (l + k > r || EQ(l, l + k, r - l - k)) //is s[l, r) periodic with period k
#define N 100010

string s;
hsh p[N], h[N], base = { dist(gen) | 1, dist(gen) | 1 };
ll n, suff[N];

ll lcp(ll i, ll j, ll l, ll r) { //for l = 0, returns length of lcp of s[i, i + r) and s[j, j + r)
    if(l == r) return l;
    ll m = (l + r + 1) / 2;
    return EQ(i, j, m) ? lcp(i, j, m, r) : lcp(i, j, l, m - 1);
}

bool lex_less(ll i, ll lI, ll j, ll lJ) { //is s[i, i + lI) lexicog. less than s[j, j + lJ)
    if(EQ(i, j, min(lI, lJ))) return lI < lJ;
    ll m = lcp(i, j, 0, min(lI, lJ) - 1);
    return s[i + m] < s[j + m];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> s;
    p[0] = { 1, 1 };
    F(i, 1, s.size() + 1) h[i] = h[i - 1] + (p[i] = p[i - 1] * base) * make_pair(s[i - 1], s[i - 1]);
    iota(suff, suff + n, 0);
    sort(suff, suff + n, [](ll i1, ll i2) { return lex_less(i1, n - i1, i2, n - i2); });
}
