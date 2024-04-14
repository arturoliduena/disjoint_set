#ifndef UNION_WEIGHT_H
#define UNION_WEIGHT_H

#include <iostream>
#include <random>
#include <vector>
#include "UnionFind.h"

using namespace std;

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class UnionWeight : public UnionFind {
public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  UnionWeight(int n, PathCompressionType pathCompressionType) {
    P = vector<int>(n, -1);
    n_blocks = n;
    this->pathCompressionType = pathCompressionType;
  };

  // Performs the union of the classes with representatives ri and rj
  void merge(int i, int j) {
    int ri = find(i);
    int rj = find(j);
    if (ri == rj) return;

    if (ri != rj) {
      if (P[ri] >= P[rj]) {
        // ri is the smallest/shortest
        P[rj] += P[ri];
        P[ri] = rj;
      }
      else {
        // rj is the smallest/shortest
        P[ri] += P[rj];
        P[rj] = ri;
      }
      --n_blocks;
    }
  }

};

#endif // UNION_WEIGHT_H