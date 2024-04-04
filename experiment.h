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
  vector<double> avg_tpl_values(n / delta + 1, 0);
  vector<double> avg_tpu_values(n / delta + 1, 0);
  vector<double> avg_cost_values(n / delta + 1, 0);
  vector<int> executions_count(n / delta + 1, 0);

  for (int t = 0; t < T; ++t) {
    int processed_pairs = 0;

    // Generate distinct pairs of elements (i, j) in random order
    vector<pair<int, int>> pairs = generate_pairs(n);

    for (const auto& pair : pairs) {
      if (!uf->connected(pair.first, pair.second)) {
        uf->merge(pair.first, pair.second);
        processed_pairs++;
        // cout << "blocks: " << uf->nr_blocks() << " ,precessed_pairs: " << processed_pairs << endl;
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

  }

  // data to be written to the CSV file
  std::vector<std::vector<std::string>> data;
  // Add header
  data.push_back({ "N", "Average TPL", "Average TPU", "Average Cost" });

  for (int i = 0; i < avg_tpl_values.size(); ++i) {
    if (executions_count[i] >= min_executions) {
      avg_tpl_values[i] /= executions_count[i];
      avg_tpu_values[i] /= executions_count[i];
      avg_cost_values[i] /= executions_count[i];
      data.push_back({ to_string(i * delta), to_string(avg_tpl_values[i]), to_string(avg_tpu_values[i]), to_string(avg_cost_values[i]) });
      cout << "N = " << i * delta << ", Average TPL = " << avg_tpl_values[i] << ", Average TPU = " << avg_tpu_values[i] << ", Average cost = " << avg_cost_values[i] << endl;
    }
  }

  // Name of the CSV file to be written
  string filename = "outputs/data.csv";

  // comments to be written to the CSV file
  std::vector<std::string> comments = {
      "This is a CSV file generated by my program.",
      "Comments can be added at the beginning of the file."
  };
  // Write data to the CSV file
  writeCSV(filename, data, comments);

  std::cout << "Data has been written to " << filename << std::endl;
}
#endif // EXPERIMENT_H
