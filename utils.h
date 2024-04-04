#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <random>
#include "UnionFind.h"

using namespace std;

// Generate distinct pairs of elements (i, j) in random order

vector<pair<int, int>> generate_pairs(int n) {
  random_device rd;
  mt19937 g(rd());
  vector<pair<int, int>> pairs;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      pairs.push_back({ i, j });
    }
  }
  shuffle(pairs.begin(), pairs.end(), g);
  return pairs;
}

int depth(const vector<int>& parent, int node, int root) {
  int depth = 0;
  while (node != root) {
    ++depth;
    node = parent[node];
  }
  return depth;
}

int measure_TPL(UnionFind& uf) {
  int total_path_length = 0;
  vector<int> P = uf.Parents();
  for (int i = 0; i < P.size(); ++i) {
    if (P[i] != i) {
      int root = uf.find(i);
      total_path_length += depth(P, i, root);
    }
  }
  return total_path_length;
}

int measure_TPU(UnionFind& uf) {
  vector<int> P = uf.Parents();
  int total_pointer_updates = 0;
  for (int i = 0; i < P.size(); ++i) {
    if (P[i] != i) {
      int root = uf.find(i);
      total_pointer_updates += depth(P, i, root);
    }
  }
  return total_pointer_updates;
}

// Helper function for recursive tree traversal
void printTreeHelper(vector<int> P, int root, int depth) {
  for (int i = 1; i < depth; ++i)
    cout << "    "; // indentation for depth

  if (depth >= 1) {
    cout << "|-- ";
  }
  cout << root << endl;
  for (int i = 0; i < P.size(); ++i) {
    if (P[i] == root && i != root) {
      printTreeHelper(P, i, depth + 1);
    }
  }
}

// Print the union-find structure as a tree
void printAsTree(UnionFind& uf) {
  cout << "Union-Find Structure as a Tree:" << endl;
  vector<int> P = uf.Parents();
  for (int i = 0; i < P.size(); ++i) {
    if (P[i] == i) {
      printTreeHelper(P, i, 0);
    }
  }
  cout << "TPL: " << uf.tpl() << endl;
  cout << "TPU: " << uf.tpu() << endl;
  cout << "totalCost: " << uf.totalCost(2) << endl;
  cout << "number of blocks: " << uf.nr_blocks() << endl;
  cout << "------------------------------" << endl;
}

// Print the union-find structure as a tree
void printUF(UnionFind& uf) {
  printAsTree(uf);
  cout << "TPL: " << uf.tpl() << endl;
  cout << "TPU: " << uf.tpu() << endl;
  cout << "totalCost: " << uf.totalCost(2) << endl;
  cout << "number of blocks: " << uf.nr_blocks() << endl;
  cout << "------------------------------" << endl;
}
#endif // UTILITIES_H
