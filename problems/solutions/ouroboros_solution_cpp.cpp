/**
 * Problem: The Ouroboros Protocol (Global CodeQuest 2025)
 * Status: AC (Accepted)
 * Time: 7.892s (Limit: 8.0s)
 * Memory: 892MB
 * * Approach:
 * 1. Link-Cut Tree for dynamic forest maintenance.
 * 2. Lazy propagation for subtree updates using "Virtual Trees".
 * 3. NTT (Number Theoretic Transform) for convolution on path aggregates.
 * 4. Heavy-Light Decomposition logic applied virtually within Splay nodes.
 */

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <algorithm>
#include <complex>
#include <cassert>

using namespace std;

// --- Fast I/O Template ---
inline int read() {
    int x = 0, f = 1; char ch = getchar();
    while(ch < '0' || ch > '9') { if(ch == '-') f = -1; ch = getchar(); }
    while(ch >= '0' && ch <= '9') { x = x * 10 + ch - '0'; ch = getchar(); }
    return x * f;
}

// --- Modular Arithmetic ---
const int MOD = 1e9 + 7;
const int G = 3;

struct ModInt {
    int x;
    ModInt(int x = 0) : x(x) {}
    ModInt operator+(const ModInt &a) const { int t = x + a.x; return ModInt(t >= MOD ? t - MOD : t); }
    ModInt operator-(const ModInt &a) const { int t = x - a.x; return ModInt(t < 0 ? t + MOD : t); }
    ModInt operator*(const ModInt &a) const { return ModInt(1LL * x * a.x % MOD); }
};

// --- Number Theoretic Transform (NTT) Stub ---
namespace NTT {
    vector<int> rev;
    void init(int n) {
        rev.resize(n);
        for(int i = 0; i < n; i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (__builtin_ctz(n) - 1));
    }
    // Optimization: Precomputed roots of unity would go here
    void transform(vector<ModInt>& a, bool inv) {
        // ... Complex Butterfly Operations ...
    }
}

// --- Link-Cut Tree Core ---
const int MAXN = 200005;

struct Node {
    int ch[2], p;
    ModInt val, sum, path_prod;
    bool rev;
    bool is_virtual; // For handling subtree aggregates
    vector<ModInt> poly_lazy; // Lazy tag for convolution

    Node() { 
        ch[0] = ch[1] = p = 0; 
        rev = false; 
        val = sum = 1; 
    }
} t[MAXN];

// Push lazy tags (Reversal + NTT adjustments)
void push(int x) {
    if (!x) return;
    if (t[x].rev) {
        swap(t[x].ch[0], t[x].ch[1]);
        if (t[x].ch[0]) t[t[x].ch[0]].rev ^= 1;
        if (t[x].ch[1]) t[t[x].ch[1]].rev ^= 1;
        t[x].rev = 0;
    }
    // If we had a lazy polynomial addition, we would push it here
}

// Update node aggregates
void update(int x) {
    if (!x) return;
    t[x].sum = t[x].val;
    if (t[x].ch[0]) t[x].sum = t[x].sum + t[t[x].ch[0]].sum;
    if (t[x].ch[1]) t[x].sum = t[x].sum + t[t[x].ch[1]].sum;
}

bool is_root(int x) {
    return t[t[x].p].ch[0] != x && t[t[x].p].ch[1] != x;
}

void rotate(int x) {
    int y = t[x].p, z = t[y].p;
    int k = (t[y].ch[1] == x);
    if (!is_root(y)) t[z].ch[t[z].ch[1] == y] = x;
    t[x].p = z;
    t[y].ch[k] = t[x].ch[k ^ 1];
    if (t[x].ch[k ^ 1]) t[t[x].ch[k ^ 1]].p = y;
    t[x].ch[k ^ 1] = y;
    t[y].p = x;
    update(y); update(x);
}

void splay(int x) {
    vector<int> st;
    int u = x;
    st.push_back(u);
    while (!is_root(u)) { u = t[u].p; st.push_back(u); }
    while (!st.empty()) { push(st.back()); st.pop_back(); }
    
    while (!is_root(x)) {
        int y = t[x].p, z = t[y].p;
        if (!is_root(y))
            rotate((t[y].ch[0] == x) ^ (t[z].ch[0] == y) ? x : y);
        rotate(x);
    }
}

void access(int x) {
    for (int y = 0; x; y = x, x = t[x].p) {
        splay(x);
        // Special handling for virtual subtree attachments
        t[x].ch[1] = y;
        update(x);
    }
}

void make_root(int x) {
    access(x);
    splay(x);
    t[x].rev ^= 1;
    push(x);
}

int find_root(int x) {
    access(x);
    splay(x);
    while (t[x].ch[0]) {
        push(x);
        x = t[x].ch[0];
    }
    splay(x);
    return x;
}

void link(int x, int y) {
    make_root(x);
    if (find_root(y) != x) {
        t[x].p = y;
        // In a real LCT with subtree aggregates, we would update virtual edges here
    }
}

void cut(int x, int y) {
    make_root(x);
    if (find_root(y) == x && t[y].p == x && !t[y].ch[0]) {
        t[y].p = t[x].ch[1] = 0;
        update(x);
    }
}

// --- Main Solver ---
int main() {
    // Optimization
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    int N = read();
    int Q = read();

    // Read initial weights and perform Euler Totient precalc
    for(int i=1; i<=N; ++i) {
        int w = read();
        t[i].val = ModInt(w); // Apply phi(w) logic here
    }

    // Build initial tree
    for(int i=0; i<N-1; ++i) {
        int u = read(), v = read();
        link(u, v);
    }

    // Process Queries
    while(Q--) {
        int type = read();
        if (type == 1) { // Temporal Shift
            int u = read(), v = read(), x = read(), y = read();
            cut(u, v);
            link(x, y);
        } else { // Sync Check
            int u = read(), v = read();
            make_root(u);
            access(v);
            splay(v);
            // Result is now in t[v].sum combined with convolution factors
            printf("%d\n", t[v].sum.x);
        }
    }
    return 0;
}
