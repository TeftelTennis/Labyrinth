#include <utility>

#ifndef DSU_H
#define DSU_H



class DSU {
private:
    int const static MAXSIZE = 1000;
public:
    int k;
    std::pair<int, int> parent[MAXSIZE][MAXSIZE];
    DSU(int w, int h);
    std::pair<int, int> color(int i, int j);
    void merge(int i1, int j1, int i2, int j2);
};


#endif // DSU_H
