//Useful STL Functions
#include <bits/stdc++.h>
using namespace std;
int a[10], b[10], n = 10, v = 6, u = 8;
set<int> st;
map<int, int> mp;

int main() {
    //returns true iff v is somewhere in a (a must be sorted)
    binary_search(a, a + n, v);
    //copies a into b
    copy(a, a + n, b);
    copy_n(a, n, b);
    //returns number of times v occurs in a
    count(a, a + n, v);
    //returns true iff a[i] == b[i] for all 0 <= i < n
    equal(a, a + n, b);
    //fills a with value v
    fill(a, a + n, v);
    fill_n(a, n, v);
    //returns iterator to first occurrence of v in a
    find(a, a + n, v);
    //sets all a[i] to v + i (useful for DSU)
    iota(a, a + n, v);
    //returns true iff first n values in a and b are permutations of each other
    is_permutation(a, a + n, b);
    //returns true iff a is nondecreasing
    is_sorted(a, a + n);
    //returns iterator to first element not in sorted order
    is_sorted_until(a, a + n);
    //returns true iff the sequence in a is lexicographically less than the sequence in b
    lexicographical_compare(a, a + n, b, b + n);
    //returns iterator to first element >= v (a must be sorted)
    lower_bound(a, a + n, v);
    st.lower_bound(v);
    mp.lower_bound(v); //note that v is the key type
    //returns iterator to max element
    max_element(a, a + n);
    //returns iterator to min element
    min_element(a, a + n);
    //returns pair of iterators to min and max
    minmax_element(a, a + n);
    //permutes elements of a to next permutation, returns true iff next / prev permutation exists
    //can be used to loop through permutations (do/while loop)
    next_permutation(a, a + n);
    prev_permutation(a, a + n);
    //replaces all occurrences of v in a with u
    replace(a, a + n, v, u);
    //reverses order of a
    reverse(a, a + n);
    //rotates values in a such that a[v] holds previous a[0]
    rotate(a, a + v, a + n);
    //randomly shuffles a
    mt19937 g(__builtin_ia32_rdtsc());
    shuffle(a, a + n, g);
    //sorts a
    sort(a, a + n);
    //swaps values of u and v
    swap(u, v);
    //swaps first n values in a and b
    swap_ranges(a, a + n, b);
    //returns iterator to first element > v (a must be sorted)
    upper_bound(a, a + n, v);
    st.upper_bound(v);
    mp.upper_bound(v); //note that v is the key type
    //do the same things as corresponding functions above, but copy result into b rather than modifying a
    replace_copy(a, a + n, b, v, u);
    reverse_copy(a, a + n, b);
    rotate_copy(a, a + v, a + n, b);
}
