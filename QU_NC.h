#ifndef QUICK_UNION_NO_COMPRESSION_H
#define QUICK_UNION_NO_COMPRESSION_H

#include <iostream>
#include <random>
#include <vector>
#include "UnionFind.h"
// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class QuickUnionNoCompression : public UnionFind {
public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  QuickUnionNoCompression(int n) {
    for (int i = 0; i < n; ++i) {
      P.push_back(i);
    };
    TPL = 0;
    TPU = 0;
    n_blocks = n;
  };

  // Returns the representative of the class to which i belongs
  int find(int i) {
    if (P[i] == i) {
      return P[i];
    }
    return find(P[i]);
  }

  // Performs the union of the classes with representatives ri and rj
  void merge(int i, int j) {
    int ri = find(i);
    int rj = find(j);
    if (ri != rj) {
      TPL += pathLength(i) + pathLength(j); // Update TPL
      P[ri] = rj;
      --n_blocks;
    }
  }

};

#endif // QUICK_UNION_NO_COMPRESSION_H