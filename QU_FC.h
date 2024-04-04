#ifndef QUICK_UNION_FULL_COMPRESSION_H
#define QUICK_UNION_FULL_COMPRESSION_H

#include <iostream>
#include <random>
#include <vector>
#include "UnionFind.h"

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class QuickUnionFullCompression : public UnionFind {
public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  QuickUnionFullCompression(int n) {
    for (int i = 0; i < n; ++i) {
      P.push_back(i);
    };
    n_blocks = n;
    TPL = 0;
    TPU = 0;
  };

  int find(int i) override {
    if (P[i] == i) return i;
    else {
      P[i] = find(P[i]);
      ++TPU; // Update TPU
      return P[i];
    }
  }

  void merge(int i, int j) override {
    int ri = find(i);
    int rj = find(j);
    if (ri != rj) {
      TPL += pathLength(i) + pathLength(j); // Update TPL
      P[ri] = rj;
      --n_blocks;
    }
  }
};

#endif // QUICK_UNION_FULL_COMPRESSION_H
