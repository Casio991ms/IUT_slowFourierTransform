#include <bits/stdc++.h>
using namespace std;
using LL = long long;
// solves CodeForces 732F

struct edge {
    int u, v;
    edge(int u = 0, int v = 0) : u(u), v(v) {}
    int to(int node){
        return u ^ v ^ node;
    }
};
struct graph {
    int n;
    vector<vector<int>> adj;
    vector <edge> edges;
    graph(int n = 0) : n(n), adj(n) {}
    void addEdge(int u, int v, bool dir = 1) {
        adj[u].push_back(edges.size());
        if(dir) adj[v].push_back(edges.size());
        edges.emplace_back(u, v);
    }
    int addNode() {
        adj.emplace_back();
        return n++;
    }
    edge &operator()(int idx) { return edges[idx]; }
    vector<int> &operator[](int u) { return adj[u]; }
};
namespace bridge_tree {
    vector<vector<int>> components;
    vector<int> depth, low;
    stack<int> st;
    vector<int> id;
    vector<edge> bridges;
    graph tree;
    void find_bridges(int node, graph &G, int par = -1, int d = 0) {
        low[node] = depth[node] = d;
        st.push(node);
        for (int id : G[node]) {
            int to = G(id).to(node);
            if (par != to) {
                if (depth[to] == -1) {
                    find_bridges(to, G, node, d + 1);
                    if (low[to] > depth[node]) {
                        bridges.emplace_back(node, to);
                        components.push_back({});
                        for (int x = -1; x != to; x = st.top(), st.pop())
                            components.back().push_back(st.top());
                    }
                }
                low[node] = min(low[node], low[to]);
            }
        }
        if (par == -1) {
            components.push_back({});
            while (!st.empty()) components.back().push_back(st.top()), st.pop();
        }
    }
    graph &create_tree() {
        for (auto &comp : components) {
            int idx = tree.addNode();
            for (auto &e : comp) id[e] = idx;
        }
        for (auto &[l, r] : bridges) tree.addEdge(id[l], id[r]);
        return tree;
    }
    void init(graph &G) {
        int n = G.n;
        depth.assign(n, -1), id.assign(n, -1), low.resize(n);
        for (int i = 0; i < n; i++)
            if (depth[i] == -1) find_bridges(i, G);
    }
}
using tree = graph;
int main() {
    ios_base :: sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    graph G(n);
    vector <edge> edges(m);
    for(auto &[u, v]: edges) {
        cin >> u >> v;
        u--, v--;
        G.addEdge(u, v);
    }
    bridge_tree :: init(G);
    tree T = bridge_tree :: create_tree();
    auto &comps = bridge_tree :: components;
    auto &id = bridge_tree :: id;
    int ans = 0;
    for(int i = 1; i < T.n; i++)
        if(comps[i].size() > comps[ans].size())
            ans = i;
    vector <int> par(n, -1), ht(n, 0);
    function <void (int)> dfs = [&](int node) {
        for(int e: G[node]) if(int to = G(e).to(node); par[to] == -1){
            par[to] = node;
            ht[to] = ht[node] + 1;
            dfs(to);
        }
    };
    par[comps[ans][0]] = comps[ans][0];
    dfs(comps[ans][0]);
    vector <int> cnt(n);
    cout << comps[ans].size() << '\n';
    for(auto &[u, v]: edges) {
        if(u == par[v]) swap(u, v);
        else if(v != par[u] and ht[v] < ht[u]) swap(u, v);
        cout << u + 1 << ' ' << v + 1 << '\n';
    }
}
