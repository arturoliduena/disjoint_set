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
    cerr << "Invalid value of type. It should like QU NC || QU FC || UR FC || UR NC || UW FC || UW NC." << endl;
    return 1; // Return error code
  }
  // Convert the string type to UFType
  UFType type = typeMap[typeStr];

  // convert the string path compression type to PathCompressionType
  PathCompressionType pctype = pathCompressionTypeMap[pctypeStr];

  int delta = 2; //stoi(argv[2]);
  const int T = 20;
  const int min_executions = 5;
  int epsilon = 2; //Cost of updating a pointer

  UnionFind* uf;

  int total = n / delta + 1;

  vector<double> avg_tpl_values(n * n, 0);
  vector<double> avg_tpu_values(n * n, 0);
  vector<int> executions_count(n * n, 0);
  for (int t = 0; t < T; ++t) {
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
    vector<pair<int, int>> pairs = generate_pairs(n, t);

    for (const auto& pair : pairs) {
      uf->merge(pair.first, pair.second);
      processed_pairs++;
      // uf->print();
      // printAsTree(uf->Parents());
      if (processed_pairs % delta == 0) {
        int index = processed_pairs / delta;
        Pair calc = uf->calculateTPL();
        avg_tpl_values[index] += calc.tpl;
        avg_tpu_values[index] += calc.tpu;
        executions_count[index]++;
      }
      if (uf->nr_blocks() == 1) {
        break;
      }
    }
    // printUF(uf);
    delete uf;
  }

  // data to be written to the CSV file
  vector<vector<string>> data;
  // Add header
  data.push_back({ "N", "Average TPL", "Average TPU" });

  for (int i = 0; i < avg_tpl_values.size(); ++i) {
    if (executions_count[i] >= min_executions) {
      avg_tpl_values[i] /= executions_count[i];
      avg_tpu_values[i] /= executions_count[i];
      data.push_back({ to_string(i * delta), to_string(avg_tpl_values[i]), to_string(avg_tpu_values[i]) });
    }
  }

  // Name of the CSV file to be written
  string filename = string("outputs/") + typeStr + "_" + pctypeStr + "_" + to_string(n) + string(".csv");

  // comments to be written to the CSV file
  vector<string> comments = {
      "This is a CSV file containing the average TPL, TPU, and Cost values for the " + typeStr + " algorithm with " + pctypeStr + " path compression.",
      "The data was generated for n = " + to_string(n) + " and delta = " + to_string(delta) + ".",
  };
  // Write data to the CSV file
  writeCSV(filename, data, comments);

  cout << "Data has been written to " << filename << endl;

  return 0;
}
