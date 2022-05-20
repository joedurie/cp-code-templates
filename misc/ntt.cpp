//Number Theoretic Transform
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define M 998244353
#define N 100010
#define L 20

ll pw(ll a, ll p) { return p ? pw(a * a % M, p / 2) * (p & 1 ? a : 1) % M : 1; }
ll inv(ll a, ll b = M) { return 1 < a ? b - inv(b % a, a) * b / a : 1; }

ll root = 62;
vector<ll> rt(2, 1);

void ntt(vector<ll> &a) {
	ll n = a.size();
	vector<ll> rev(n);
	F(i, 0, n) if(i < (rev[i] = (rev[i / 2] | (i & 1 ? n : 0)) / 2)) swap(a[i], a[rev[i]]);
	for(ll k = 1; k < n; k *= 2)
		for(ll i = 0; i < n; i += 2 * k) F(j, 0, k) {
			ll z = rt[j + k] * a[i + j + k] % M, &ai = a[i + j];
			a[i + j + k] = (ai - z + M) % M;
			ai = (ai + z) % M;
		}
}

vector<ll> mult(vector<ll> a, vector<ll> b) {
	ll n = a.size() + b.size();
    while(__builtin_popcountll(n) > 1) n++;
    a.resize(n), b.resize(n);
    vector<ll> out(n); ll in = inv(n);
	ntt(a), ntt(b);
	F(i, 0, n) out[-i & (n - 1)] = a[i] * b[i] % M * in % M;
	ntt(out);
	return out;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    F(s, 2, L) {
		ll z[] = {1, pw(root, M >> s)};
		F(i, 1 << (s - 1), 1 << s) rt.push_back(rt[i / 2] * z[i & 1] % M);
	}
}
