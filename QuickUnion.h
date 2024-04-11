#ifndef QUICK_UNION_H
#define QUICK_UNION_H

#include <iostream>
#include <random>
#include <vector>
#include "UnionFind.h"

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class QuickUnion : public UnionFind {
public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  QuickUnion(int n, PathCompressionType type) {
    for (int i = 0; i < n; ++i) {
      P.push_back(i);
    };
    n_blocks = n;
    this->type = type;
  };

  void merge(int i, int j) override {
    int ri = find(i);
    int rj = find(j);
    if (ri != rj) {
      P[ri] = rj;
      --n_blocks;
    }
  }
};

#endif // QUICK_UNION_H
