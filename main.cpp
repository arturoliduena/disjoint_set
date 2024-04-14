#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include "utils.h"
#include "UnionFind.h"
#include "QuickUnion.h"
#include "UnionWeight.h"
#include "UnionRank.h"

using namespace std;

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

// Define an enum for the types
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
  if (argc != 6) {
    cerr << "Usage: " << argv[0] << " <UF>" << " <path compression>" << " <n>" << " <delta>" << " <seed>" << endl;
    return 1; // return error code
  }
  // Get the type from command-line arguments
  string typeStr = argv[1];
  string pctypeStr = argv[2];

  // Ensure that the input type is valid
  if (typeMap.find(typeStr) == typeMap.end()) {
    cerr << "Invalid value of type. It should like QU or UR or UW." << endl;
    return 1; // Return error code
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
  int delta;
  try {
    delta = stoi(argv[4]);
  }
  catch (const std::invalid_argument& e) {
    cerr << "Invalid argument: " << argv[4] << endl;
    return 1; // Return error code
  }

  int seed;
  try {
    seed = stoi(argv[5]);
  }
  catch (const std::invalid_argument& e) {
    cerr << "Invalid argument: " << argv[5] << endl;
    return 1; // Return error code
  }
  // Convert the string type to UFType
  UFType type = typeMap[typeStr];

  // convert the string path compression type to PathCompressionType
  PathCompressionType pctype = pathCompressionTypeMap[pctypeStr];

  // data to be written to the CSV file
  vector<vector<string>> data;

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
  vector<pair<int, int>> pairs = generate_pairs(n, seed);

  for (const auto& pair : pairs) {
    uf->merge(pair.first, pair.second);
    processed_pairs++;
    // uf->print();
    // printAsTree(uf->Parents());
    if (processed_pairs % delta == 0) {
      int index = processed_pairs / delta;
      Measure calc = uf->calculateTPL();
      data.push_back({ to_string(processed_pairs), to_string(calc.tpl), to_string(calc.tpu), typeStr, pctypeStr });
    }
    if (uf->nr_blocks() == 1) {
      break;
    }
  }
  // printUF(uf);
  delete uf;

  // Write data to standard output
  for (const auto& row : data) {
    for (const auto& col : row) {
      cout << col;
      if (&col != &row.back())
        cout << ",";
    }
    cout << endl;
  }

  return 0;
}
