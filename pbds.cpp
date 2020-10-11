//Policy-Based Data Structures
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;
using namespace __gnu_cxx;
using namespace __gnu_pbds;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define N 100010

//Order Statistics Tree Setup
typedef tree<ll, null_type, less<ll>,
rb_tree_tag, tree_order_statistics_node_update> set_t;

//Hash Table Setup 
ll seed = __builtin_ia32_rdtsc();
struct chash { ll operator()(ll x) const { return x ^ seed; } };
typedef gp_hash_table<ll, ll, chash> table_t;

int main() {
    //Order Statistics Tree
    set_t s;
    s.insert(6), s.insert(11);
    cout << *s.find_by_order(1) << '\n'; //element at (0-indexed) position 1
    cout << s.order_of_key(9) << '\n'; //number of elements in s that are < 9
    cout << s.size() << '\n';
    cout << (s.find(6) != s.end()) << ' ' << (s.find(7) != s.end()) << '\n';
    s.erase(11);
    for(ll x : s) cout << x << ' ';
    cout << '\n';

    //Hash Table
    table_t t; //can be used basically as std::unordered_map
    t[2222222] = 17;
    cout << t[2222222] << ' ' << t[2345678] << '\n';
    cout << (t.find(2345678) != t.end()) << ' ' << (t.find(1234567) != t.end()) << '\n';
    t.erase(2345678);
    for(pl p : t) cout << p.first << ' ' << p.second << '\n';

    //Rope
    rope<char> r;
    F(i, 0, 26) r.push_back('a' + i);
    r.erase(3, 14); //erases substring of r of size 14 starting at index 3 
    rope<char> sub = r.substr(1, 8); //substring of r of size 8 starting at index 1
    r.insert(r.mutable_begin(), sub); //appends sub to start of r
    r.insert(r.mutable_end(), sub); //appends sub to end of r
    cout << r.size() << '\n';
    cout << r[5] << '\n';
    for(char c : r) cout << c;
    cout << '\n';
}
