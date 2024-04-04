#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include "experiment.h"
#include "UnionFind.h"
#include "QU_NC.h"
#include "QU_FC.h"

using namespace std;

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

// Define an enum for the types
enum UFType { QU_NC, QU_FC };

int main(int argc, char* argv[]) {
  // check if an argument is provided
  if (argc != 3) {
    cerr << "Usage: " << argv[1] << "<integer argument>" << " <string argument>" << endl;
    return 1; // return error code
  }
  // Convert the argument to an integer
  int n;
  try {
    n = stoi(argv[1]);
  }
  catch (const std::invalid_argument& e) {
    cerr << "Invalid argument: " << argv[1] << endl;
    return 1; // Return error code
  }
  // Define a mapping from string to UFType
  unordered_map<string, UFType> typeMap = {
      {"QU_NC", QU_NC},
      {"QU_FC", QU_FC}
  };

  // Get the type from command-line arguments
  string typeStr = argv[2];
  // Ensure that the input type is valid
  if (typeMap.find(typeStr) == typeMap.end()) {
    cerr << "Invalid value of type. It should be QU_NC or QU_FC." << endl;
    return 1; // Return error code
  }
  // Convert the string type to UFType
  UFType type = typeMap[typeStr];

  int delta = 1; //stoi(argv[2]);
  const int T = 20;
  const int min_executions = 1;
  int epsilon = 2; //Cost of updating a pointer

  UnionFind* uf;
  switch (type) {
  case QU_NC:
    uf = new QuickUnionNoCompression(n);
    break;
  case QU_FC:
    uf = new QuickUnionFullCompression(n);
    break;
  }
  // Generate distinct pairs of elements (i, j) in random order
  vector<pair<int, int>> pairs = generate_pairs(n);
  run_experiment(uf, n, pairs, delta, T, min_executions, epsilon);
  delete uf;
  return 0;
}
