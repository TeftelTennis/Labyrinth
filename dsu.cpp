#include "dsu.h"

using namespace std;

DSU::DSU(int w, int h) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            parent[i][j] = make_pair(i, j);
        }
    }
    k = w * h;
}

pair<int, int> DSU::color(int i, int j) {
    pair<int, int> tmp;
    if (parent[i][j] == make_pair(i, j)) {
        return make_pair(i, j);
    } else {
        tmp = color(parent[i][j].first, parent[i][j].second);
        parent[i][j] = tmp;
        return tmp;
    }
}

void DSU::merge(int i1, int j1, int i2, int j2) {
    pair<int, int> a = color(i1, j1);
    pair<int, int> b = color(i2, j2);
    if (a == b) {
        return;
    }
    k--;
    parent[b.first][b.second] = a;
}
