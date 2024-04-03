#include <iostream>
#include <random>
#include <vector>
#include "utils.h"
#include "UnionFind.h"

// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.
using namespace std;
class QuickUnion : public UnionFind {
private:
  vector<int> children_count; // Number of children of each root
public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  QuickUnion(int n) : children_count(vector<int>(n, 0)) {
    for (int i = 0; i < n; ++i) {
      P.push_back(i);
    };
    n_blocks = n;
  };

  int find(int i) override {
    if (P[i] == i) return i;
    else {
      P[i] = find(P[i]);
      return P[i];
    }
  }

  void merge(int i, int j) override {
    int ri = find(i);
    int rj = find(j);
    if (ri != rj) {
      P[ri] = rj;
      // Update number of children of the new root
      children_count[rj] += (children_count[ri] + 1);
      --n_blocks;
    }
  }

  int depth(const vector<int>& parent, int node, int root) {
    int depth = 0;
    while (node != root) {
      ++depth;
      node = parent[node];
    }
    return depth;
  }

  int get_tpl() {
    int total_path_length = 0;
    for (int i = 0; i < P.size(); ++i) {
      if (P[i] != i) {
        int root = find(i);
        total_path_length += depth(P, i, root);
      }
    }
    return total_path_length;
  }

  int get_tpu() {
    int total_number_children = 0;

    for (int i = 0; i < P.size(); ++i) {
      if (P[i] == i) {
        total_number_children += children_count[i];
      }
    }
    cout << "total_number_children: " << total_number_children << endl;
    return get_tpl() - total_number_children;
  }
};


int main(int argc, char* argv[]) {
  // check if an argument is provided
  if (argc != 2) {
    cerr << "Usage: " << argv[1] << "<integer argument>" << endl;
    return 1; // return error code
  }
  // convert the argument to an integer
  int n = stoi(argv[1]);
  const int Delta = 1; // Delta value as specified

  vector<int> TPL_values;
  vector<int> TPU_values;

  for (int t = 0; t < 1; ++t) { // T = 1 times
    QuickUnion uf(n);
    int processed_pairs = 0;

    // Generate distinct pairs of elements (i, j) in random order
    vector<pair<int, int>> pairs = generate_pairs(n);

    for (const auto& pair : pairs) {
      if (!uf.connected(pair.first, pair.second)) {
        uf.merge(pair.first, pair.second);
        processed_pairs++;
        printAsTree(uf);
        cout << "TPL: " << uf.get_tpl() << endl;
        cout << "TPU: " << uf.get_tpu() << endl;
        cout << "------------------------------" << endl;
      }
      if (uf.nr_blocks() == 1) {
        break;
      }
    }


    int epsilon = 2; // Example: Cost of updating a pointer
    // Calculate Total Cost
    int totalCost = 0; // uf.totalCost(epsilon);

    cout << "number of blocks: " << uf.nr_blocks() << ", total cost:" << totalCost << ", TPL:" << uf.get_tpl() << ", TPU: " << uf.get_tpu() << endl;
    // uf.printAsTree();

    // if (processed_pairs >= Delta) {
    //   int TPL = measure_TPL(uf);
    //   int TPU = measure_TPU(uf);
    //   TPL_values.push_back(TPL);
    //   TPU_values.push_back(TPU);
    // }
  }

  // Calculate averages for TPL and TPU as functions of N
  vector<int> avg_TPL(n / Delta + 1, 0);
  vector<int> avg_TPU(n / Delta + 1, 0);

  for (int i = 0; i < TPL_values.size(); ++i) {
    avg_TPL[i / Delta] += TPL_values[i];
    avg_TPU[i / Delta] += TPU_values[i];
  }

  // for (int i = 0; i < avg_TPL.size(); ++i) {
  //   avg_TPL[i] /= 20; // Divide by the number of experiments (T = 20)
  //   avg_TPU[i] /= 20;
  //   cout << "For N = " << i * Delta << ", Avg TPL: " << avg_TPL[i] << ", Avg TPU: " << avg_TPU[i] << endl;
  // }

  return 0;
}
