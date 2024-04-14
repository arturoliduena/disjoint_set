#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <iostream>
#include <vector>
#include <cmath>

// Define an enum for the types
enum PathCompressionType { NC, FC, PS, PH };

struct Measure {
  int tpl;
  int tpu;
};

class UnionFind {
protected:
  std::vector<int> P;
  int n_blocks;
  PathCompressionType pathCompressionType;
public:
  // Default constructor
  UnionFind() : n_blocks(0) {}

  // Virtual destructor
  virtual ~UnionFind() {}

  // Returns the representative of the class to which i belongs
  virtual int find(int i) {
    if (P[i] == i || P[i] < 0) return i;

    //no compression
    if (pathCompressionType == NC) {
      return find(P[i]);;
    }

    //full path compression: Make every node point to its representative.
    else if (pathCompressionType == FC) {
      P[i] = find(P[i]);
      return P[i];
    }

    //path splitting: Make every node point to its grandparent (except if it is the root or a child of the root).
    else if (pathCompressionType == PS) {
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
    else if (pathCompressionType == PH) {
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
    else {
      std::cerr << "Invalid path compression pathCompressionType" << std::endl;
      return -1;
    }
  }

  // Performs the union of the classes with representatives ri and rj
  virtual void merge(int i, int j) = 0;

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

  Measure calculateTPL() {
    int tpl = 0;
    int tpu = 0;
    for (int i = 0; i < P.size(); ++i) {
      int const pl = pathLength(i);
      int pu = 0;
      //no compression
      if (pathCompressionType == NC) {
        pu = 0;
      }

      //full path compression: Make every node point to its representative.
      //path splitting: Make every node point to its grandparent (except if it is the root or a child of the root).
      else if (pathCompressionType == FC || pathCompressionType == PS) {
        pu = pl > 0 ? pl - 1 : 0;
      }
      //path halving: Make every other node in the path point to its grandparent (except if it is the root or a child of the root).
      else if (pathCompressionType == PH) {
        pu = floor(pl / 2);
      }
      else {
        std::cerr << "Invalid path compression pathCompressionType" << std::endl;
      }
      tpl += pl;
      tpu += pu;
    }
    Measure measure = { tpl, tpu };
    return measure;
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

