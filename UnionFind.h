#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <iostream>
#include <vector>
#include <cmath>

// Define an enum for the types
enum PathCompressionType { NC, FC, PS, PH };

struct Pair {
  int tpl;
  int tpu;
};

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
  PathCompressionType type;
public:
  // Default constructor
  UnionFind() : n_blocks(0) {}

  // Virtual destructor
  virtual ~UnionFind() {}

  // Pure virtual functions for interface

  // Returns the representative of the class to which i belongs
  virtual int find(int i) {
    if (P[i] == i || P[i] < 0) return i;

    //no compression
    if (type == NC) {
      return find(P[i]);;
    }

    //full path compression: Make every node point to its representative.
    else if (type == FC) {
      P[i] = find(P[i]);
      return P[i];
    }

    //path splitting: Make every node point to its grandparent (except if it is the root or a child of the root).
    else if (type == PS) {
      int child = -1;
      int grandchild = -1;
      while (!(P[i] == i || P[i] < 0)) {
        if (grandchild != -1) {
          P[grandchild] = i;
        }
        grandchild = child;
        child = i;
        i = P[i];
      }
      return i;
    }
    //path halving: Make every other node in the path point to its grandparent (except if it is the root or a child of the root).
    else if (type == PH) {
      int distance = 0; // Distance from the root
      int grandchild = i;
      while (!(P[i] == i || P[i] < 0)) {
        if (distance == 2) {
          P[grandchild] = i;
          distance = 0;
          grandchild = i;
        }
        ++distance;
        i = P[i];
      }
      return i;
    }
  }

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

  Pair calculateTPL() {
    int tpl = 0;
    int tpu = 0;
    for (int i = 0; i < P.size(); ++i) {
      int const pl = pathLength(i);
      int pu = 0;
      //no compression
      if (type == NC) {
        pu = 0;
      }

      //full path compression: Make every node point to its representative.
      //path splitting: Make every node point to its grandparent (except if it is the root or a child of the root).
      else if (type == FC || type == PS) {
        pu = pl > 0 ? pl - 1 : 0;
      }
      //path halving: Make every other node in the path point to its grandparent (except if it is the root or a child of the root).
      else if (type == PH) {
        pu = floor(pl / 2);
      }
      else {
        std::cerr << "Invalid path compression type" << std::endl;
      }
      tpl += pl;
      tpu += pu;
    }
    Pair pair = { tpl, tpu };
    return pair;
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
    std::cout << "[ ";
    for (int num : P) {
      std::cout << num << " ";
    }
    std::cout << "]; ";
    std::cout << std::endl;
  }

};

#endif // UNIONFIND_H

