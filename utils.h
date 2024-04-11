#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include "UnionFind.h"

using namespace std;

// Generate distinct pairs of elements (i, j) in random order for the union-find algorithms
vector<pair<int, int>> generate_pairs(int n, int seed) {
  mt19937 g(seed);
  vector<pair<int, int>> pairs;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      pairs.push_back({ i, j });
    }
  }
  shuffle(pairs.begin(), pairs.end(), g);
  return pairs;
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
}

// Function to write data to a CSV file
void writeCSV(const string& filename, const vector<vector<string>>& data, const vector<string>& comments = {}) {
  // Open the file for writing
  ofstream file(filename);

  // Check if the file is opened successfully
  if (!file.is_open()) {
    cerr << "Error: Unable to open file: " << filename << endl;
    return;
  }

  // Write comments to the file
  for (const auto& comment : comments) {
    file << "#" << comment << std::endl;
  }

  // Write data to the file
  for (const auto& row : data) {
    for (size_t i = 0; i < row.size(); ++i) {
      file << row[i];
      // Add comma if it's not the last element in the row
      if (i < row.size() - 1)
        file << ",";
    }
    // Add newline character after each row
    file << endl;
  }

  // Close the file
  file.close();
}

#endif // UTILITIES_H
