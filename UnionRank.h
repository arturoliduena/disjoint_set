#ifndef UNION_RANK_H
#define UNION_RANK_H

#include <iostream>
#include <random>
#include <vector>
#include "UnionFind.h"

using namespace std;

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class UnionRank : public UnionFind {
public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  UnionRank(int n, PathCompressionType type) {
    P = vector<int>(n, -1);
    n_blocks = n;
    this->type = type;
  };

  // Performs the union of the classes with representatives ri and rj
  void merge(int i, int j) {
    int ri = find(i);
    int rj = find(j);
    if (ri == rj) return;

    if (P[ri] >= P[rj]) {
      // ri is the smallest/shortest
      P[rj] = min(P[rj], P[ri] - 1);
      P[ri] = rj;
    }
    else {
      // rj is the smallest/shortest
      P[ri] = min(P[ri], P[rj] - 1);
      P[rj] = ri;
    }
    --n_blocks;
  }

};

#endif // UNION_RANK_H