#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include "experiment.h"
#include "UnionFind.h"
#include "QuickUnion.h"
#include "UnionWeight.h"
#include "UnionRank.h"

using namespace std;

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

// Define an enum for the types
enum UFType { QU, UR, UW };

// Define a mapping from string to UFType
unordered_map<string, UFType> typeMap = {
    {"QU", QU},
    {"UR", UR},
    {"UW", UW},
};

// Define a mapping from string to PathCompressionType
unordered_map<string, PathCompressionType> pathCompressionTypeMap = {
    {"NC", NC},
    {"FC", FC},
    {"PS", PS},
    {"PH", PH},
};

int main(int argc, char* argv[]) {
  // check if an argument is provided
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << "<string argument>" << "<string argument>" << " <integer argument>" << endl;
    return 1; // return error code
  }
  // Convert the argument to an integer
  int n;
  try {
    n = stoi(argv[3]);
  }
  catch (const std::invalid_argument& e) {
    cerr << "Invalid argument: " << argv[3] << endl;
    return 1; // Return error code
  }


  // Get the type from command-line arguments
  string typeStr = argv[1];
  string pctypeStr = argv[2];
  // Ensure that the input type is valid
  if (typeMap.find(typeStr) == typeMap.end()) {
    cerr << "Invalid value of type. It should be QU_NC || QU_FC || UR_FC || UR_NC || UW_FC || UW_NC." << endl;
    return 1; // Return error code
  }
  // Convert the string type to UFType
  UFType type = typeMap[typeStr];

  // convert the string path compression type to PathCompressionType
  PathCompressionType pctype = pathCompressionTypeMap[pctypeStr];

  UnionFind* uf;

  int processed_pairs = 0;
  switch (type) {
  case QU:
    uf = new QuickUnion(n, pctype);
    break;
  case UW:
    uf = new UnionWeight(n, pctype);
    break;
  case UR:
    uf = new UnionRank(n, pctype);
    break;
  }

  // Generate distinct pairs of elements (i, j) in random order
  vector<pair<int, int>> pairs = generate_pairs(n);

  for (const auto& pair : pairs) {
    uf->print();
    if (!uf->connected(pair.first, pair.second)) {
      uf->merge(pair.first, pair.second);
      processed_pairs++;
      // printUF(uf);
    }
    if (uf->nr_blocks() == 1) {
      break;
    }
  }
  // printUF(uf);
  uf->print();
  delete uf;
  std::cout << "finish" << std::endl;

  return 0;
}
