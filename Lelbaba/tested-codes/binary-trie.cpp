#include <bits/stdc++.h>
using namespace std;
using LL = long long;
void maximize(int &a, int b) {
    a = max(a, b);
}
namespace trie {
    const int N = 1e7 + 5, b = 30;
    int tc = 1;
    struct node{
        int vis = 0;
        int to[2] = {0, 0};
        int val[2] = {0, 0};
        void update() {
            to[0] = to[1] = 0;
            val[0] = val[1] = 0;
            vis = tc;
        }
    } T[N + 2];
    node *root = T;
    int ptr = 0;
    node* nxt(node* cur, int x) {
        if(cur -> to[x] == 0) cur -> to[x] = ++ptr;
        assert(ptr < N);
        int idx = cur -> to[x];
        if(T[idx]. vis < tc) T[idx].update();
        return T + idx;
    }
    int query(int j, int aj) {
        int ans = 0, jaj = j ^ aj;
        node *cur = root;
        for(int k = b - 1; ~k; k--) {
            maximize(ans, nxt(cur, (jaj >> k & 1) ^ 1) -> val[1 ^ (aj >> k & 1)]);
            cur = nxt(cur, (jaj >> k & 1));
        }
        return ans;
    }
    void insert(int j, int aj, int val) {
        int jaj = j ^ aj;
        node *cur = root;
        for(int k = b - 1; ~k; k--) {
            cur = nxt(cur, (jaj >> k & 1));
            maximize(cur -> val[j >> k & 1], val);
        }
    }
    void clear() {
        tc++;
        ptr = 0;
        root -> update();
    }
};
int main() {
    ios_base :: sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    while(t--) {
        int n;
        cin >> n;
        vector <int> a(n);
        int ans = 0;
        for(int i = 0; i < n; i++) {
            cin >> a[i];
            int val = trie :: query(i, a[i]) + 1;
            ans = max(ans, val);
            trie :: insert(i, a[i], val);
        }
        cout << ans << '\n';
        trie :: clear();
    }
}
