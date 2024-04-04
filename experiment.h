#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "utils.h"
#include "UnionFind.h"

using namespace std;

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

void run_experiment(UnionFind* uf, int n, int delta, int T, int min_executions, int epsilon) {
  //to be implemented
}

#endif // EXPERIMENT_H
