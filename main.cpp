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
enum UFType { QU, UR, UW };

vector<UFType> UFs = { QU, UR, UW };
vector<PathCompressionType> types = { NC, FC, PS, PH };

// Function to convert PathCompressionType enum to string
string pathCompressionTypeToString(PathCompressionType type) {
  switch (type) {
  case NC: return "NC";
  case FC: return "FC";
  case PS: return "PS";
  case PH: return "PH";
  default: return "Unknown";
  }
}

// Function to convert UFType enum to string
string UFTypeToString(UFType type) {
  switch (type) {
  case QU: return "QU";
  case UR: return "UR";
  case UW: return "UW";
  default: return "Unknown";
  }
}


int main(int argc, char* argv[]) {
  // check if an argument is provided
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <integer argument>" << endl;
    return 1; // return error code
  }
  // Convert the argument to an integer
  int n;
  try {
    n = stoi(argv[1]);
  }
  catch (const invalid_argument& e) {
    cerr << "Invalid argument: " << argv[1] << endl;
    return 1; // Return error code
  }

  int delta = 1; //stoi(argv[2]);
  const int T = 20;
  const int min_executions = 5;
  int epsilon = 2; //Cost of updating a pointer

  UnionFind* uf;

  int total = n / delta + 1;

  for (const auto& UF : UFs) {
    for (const auto& type : types) {
      vector<double> avg_tpl_values(total, 0);
      vector<double> avg_tpu_values(total, 0);
      vector<double> avg_cost_values(total, 0);
      vector<int> executions_count(total, 0);
      for (int t = 0; t < T; ++t) {
        int processed_pairs = 0;
        switch (UF) {
        case QU:
          uf = new QuickUnion(n, type);
          break;
        case UW:
          uf = new UnionWeight(n, type);
          break;
        case UR:
          uf = new UnionRank(n, type);
          break;
        }

        // Generate distinct pairs of elements (i, j) in random order
        vector<pair<int, int>> pairs = generate_pairs(n, t);

        for (const auto& pair : pairs) {
          if (!uf->connected(pair.first, pair.second)) {
            uf->merge(pair.first, pair.second);
            processed_pairs++;
            // printUF(uf);
            if (processed_pairs % delta == 0 || processed_pairs == n) {
              int index = processed_pairs / delta;
              avg_tpl_values[index] += uf->tpl();
              avg_tpu_values[index] += uf->tpu();
              avg_cost_values[index] += uf->totalCost(epsilon);
              executions_count[index]++;
            }
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
      data.push_back({ "N", "Average TPL", "Average TPU", "Average Cost" });

      for (int i = 0; i < avg_tpl_values.size(); ++i) {
        if (executions_count[i] >= min_executions) {
          avg_tpl_values[i] /= executions_count[i];
          avg_tpu_values[i] /= executions_count[i];
          avg_cost_values[i] /= executions_count[i];
          data.push_back({ to_string(i * delta), to_string(avg_tpl_values[i]), to_string(avg_tpu_values[i]), to_string(avg_cost_values[i]) });
        }
      }

      // Name of the CSV file to be written
      string filename = string("outputs/") + UFTypeToString(UF) + "_" + pathCompressionTypeToString(type) + "_" + to_string(n) + string(".csv");

      // comments to be written to the CSV file
      vector<string> comments = {
          "This is a CSV file containing the average TPL, TPU, and Cost values for the " + UFTypeToString(UF) + " algorithm with " + pathCompressionTypeToString(type) + " path compression.",
          "The data was generated for n = " + to_string(n) + " and delta = " + to_string(delta) + ".",
      };
      // Write data to the CSV file
      writeCSV(filename, data, comments);

      cout << "Data has been written to " << filename << endl;
    }
  }

  return 0;
}
