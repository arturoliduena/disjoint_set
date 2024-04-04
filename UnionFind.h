#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <iostream>
#include <vector>

class UnionFind {
private:
  int _find(int i) {
    if (P[i] == i || P[i] < 0) return i;
    else {
      return _find(P[i]);
    }
  }
protected:
  std::vector<int> P;
  int n_blocks;
  int TPL; // Total Path Length (sum of path lengths from each node to its root)
  int TPU; // Total Pointer Updates
public:
  // Default constructor
  UnionFind() : n_blocks(0) {}

  // Virtual destructor
  virtual ~UnionFind() {}

  // Pure virtual functions for interface

  // Returns the representative of the class to which i belongs
  virtual int find(int i) = 0;

  // Performs the union of the classes with representatives ri and rj
  virtual void merge(int i, int j) = 0;

  virtual bool connected(int p, int q) {
    return _find(p) == _find(q);
  }

  // Returns the number of blocks in the union-find set
  virtual int nr_blocks() const {
    return n_blocks;
  };

  virtual std::vector<int> Parents() const {
    return P;
  }

  // Function to calculate path length from node i to its root
  int pathLength(int i) {
    int length = 0;
    while (!(P[i] == i || P[i] < 0)) {
      length++;
      i = P[i];
    }
    return length;
  }

  int tpl() const {
    return TPL;
  }

  int tpu() const {
    return TPU;
  }

  // Function to calculate Total Cost
  int totalCost(int epsilon) {
    return 2 * TPL + epsilon * TPU;
  }

  // Default implementation of print
  virtual void print() {
    for (int num : P) {
      std::cout << num << " ";
    }
    std::cout << std::endl;
  }

};

#endif // UNIONFIND_H

