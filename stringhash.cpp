//String Hashing
#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;

#define M1 1000000123
#define M2 1000000321
#define K first
#define V second
#define OP(a, b, c) constexpr pl operator a (const pl p, const pl q) { return { (p.K b q.K) % M1, (p.V c q.V) % M2 }; }
OP(+, +, +) OP(*, *, *) OP(-, + M1 -, + M2 -)
mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<ll> dist(256, M1 - 1);

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < r; i++)
#define HSH(l, r) ((h[r] - h[l]) * ip[l]) //substring [l, r)
#define EQ(i, j, l) (HSH(i, i + l) == HSH(j, j + l)) //true iff [i, i + l) == [j, j + l)
#define PERIODIC(l, r, k) (l + k > r || EQ(l, l + k, r - l - k)) //true iff [l, r) is periodic with period k
#define N 100010 //be careful - make sure s.size() < N after all operations

string s;
pl ip[N], h[N];
ll n, suff[N];

ll inv(ll a, ll b) { return 1 < a ? b - inv(b % a, a) * b / a : 1; }

ll lcp(ll l, ll r, ll i, ll j) { //for l = 0, returns length of longest common prefix of [i, i + r) and [j, j + r)
    if(l == r) return l;
    ll m = (l + r + 1) / 2;
    return EQ(i, j, m) ? lcp(m, r, i, j) : lcp(l, m - 1, i, j);
}

bool lex_less(ll i, ll j, ll l) { //true iff [i, i + l) lexicog. less than [j, j + l)
    ll m = lcp(0, l - 1, i, j);
    return s[i + m] < s[j + m];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n >> s;
    F(i, 0, n) s += '$';
    pl base = { dist(gen), dist(gen) };
    pl temp = { inv(base.K, M1), inv(base.V, M2) };
    ip[0] = { 1, 1 };
    F(i, 1, s.size() + 1) ip[i] = ip[i - 1] * temp;
    F(i, 1, s.size() + 1) h[i] = h[i - 1] + (temp = temp * base) * make_pair(s[i - 1], s[i - 1]);
    iota(suff, suff + n, 0);
    sort(suff, suff + n, [](ll i1, ll i2) { return lex_less(i1, i2, n); });
}
