//ICPC Template
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

#define OS ostream&
#define TP template
#define CS class
#define K first
#define V second

TP<CS T, CS S>
OS operator<< (OS o, pair<T, S> p) {
    return o << '(' << p.K << ", " << p.V << ')';
}
TP<TP<CS, CS...> CS T, CS... A>
typename enable_if<!is_same<T<A...>, string>(), OS>::type
operator<< (OS o, T<A...> v) {
    o << '[';
    for(auto a : v) o << a << ", ";
    return o << ']';
}
#define D(x) cerr << "[" << #x << "] = " << x << '\n';

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define A(x) (x).begin(), (x).end()
#define N 100010

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
}
