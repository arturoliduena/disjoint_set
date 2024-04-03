#include <iostream>
#include <random>
#include <vector>
#include "utils.h"
#include "experiment.h"
#include "UnionFind.h"
// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class QuickUnion: public UnionFind {
  public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  QuickUnion(int n) {
    for(int i = 0; i < n; ++i){
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

int main(int argc, char* argv[]) {
  // check if an argument is provided
  if(argc != 2) {
    std::cerr << "Usage: " << argv[1] << "<integer argument>" << std::endl;
    return 1; // return error code
  }
  // convert the argument to an integer
  int n = std::stoi(argv[1]);
  int delta = 1; //std::stoi(argv[2]);
  const int T = 20;
  const int min_executions = 5;
  int epsilon = 2; // Example: Cost of updating a pointer

  run_experiment<QuickUnion>(n, delta, T, min_executions, epsilon);

  return 0;
}
