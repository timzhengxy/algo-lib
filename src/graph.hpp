#pragma once

#include <algorithm>
#include <vector>
#include <numeric>
#include <limits>

using namespace std;

template <typename T>
struct PriorityQueue {

    int n;
    vector<int>& a;
    vector<T>& d;
    vector<int>& pos;

    PriorityQueue(vector<int>& _a, vector<T>& _d, vector<int>& _pos) : n((int)_a.size()), a(_a), d(_d), pos(_pos) {}

    void minHeapify(int i) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        if (l < n && d[a[l]] < d[a[smallest]])
            smallest = l;
        if (r < n && d[a[r]] < d[a[smallest]])
            smallest = r;
        if (smallest != i) {
            swap(a[i], a[smallest]);
            swap(pos[a[i]], pos[a[smallest]]);
            minHeapify(smallest);
        }
    }

    int extractMin() {
        int res = a[0];
        a[0] = a[n - 1];
        pos[a[0]] = 0;
        n--;
        minHeapify(0);
        return res;
    }

    void decreaseKey(int i, T key) {
        d[i] = key;
        int p = pos[i];
        while (p >= 0 && d[a[(p - 1) / 2]] > d[a[p]]) {
            swap(a[p], a[(p - 1) / 2]);
            swap(pos[a[p]], pos[a[(p - 1) / 2]]);
            p = (p - 1) / 2;
        }
    }

    bool empty() {return n == 0;}

};

template <typename T>
struct Graph {

    int n;
    const vector<vector<pair<int, T>>>& adj;
    
    Graph(const vector<vector<pair<int, T>>>& _adj) : n((int)_adj.size()), adj(_adj) {}
    
    vector<T> dijkstra(int s) const {
        vector<int> a(n);
        iota(a.begin(), a.end(), 0);
        swap(a[0], a[s]);

        vector<T> d(n, numeric_limits<T>::max());
        d[s] = 0;

        vector<int> pos(a);

        PriorityQueue<T> pq(a, d, pos);
        while (!pq.empty()) {
            int u = pq.extractMin();
            for (auto [v, w] : adj[u]) {
                if (d[u] + w < d[v])
                    pq.decreaseKey(v, d[u] + w);
            }
        }
        return d;
    }

};
