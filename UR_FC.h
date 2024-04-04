#ifndef UNION_RANK_FULL_COMPRESSION_H
#define UNION_RANK_FULL_COMPRESSION_H

#include <iostream>
#include <random>
#include <vector>
#include "UnionFind.h"

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class UnionRankFullCompression : public UnionFind {
public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  UnionRankFullCompression(int n) {
    P = std::vector<int>(n, -1);
    n_blocks = n;
    TPL = 0;
    TPU = 0;
  };

  // Returns the representative of the class to which i belongs
  int find(int i) {
    if (P[i] < 0) return i;
    else {
      P[i] = find(P[i]);
      ++TPU; // Update TPU
      return P[i];
    }
  }

  // Performs the union of the classes with representatives ri and rj
  void merge(int i, int j) {
    int ri = find(i);
    int rj = find(j);
    if (ri != rj) {
      TPL += pathLength(i) + pathLength(j); // Update TPL
      if (P[ri] >= P[rj]) {
        // ri is the smallest/shortest
        P[rj] = std::min(P[rj], P[ri] - 1);
        P[ri] = rj;
        --n_blocks;
      }
      else {
        // rj is the smallest/shortest
        P[ri] = std::min(P[ri], P[rj] - 1);
        P[ri] = rj;
        --n_blocks;
      }
    }
  }

};

#endif // UNION_RANK_FULL_COMPRESSION_H