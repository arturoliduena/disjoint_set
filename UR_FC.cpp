#include <iostream>
#include <random>
#include <vector>


// QU: unweighted quick-union, (2) UW: union-by-size (a.k.a. union-by-weight), (3) UR: union-by-rank. The choices for path compression are: (1) NC: no compression, (2) FC: full path compression, (3) PS: path splitting, (4) PH: path halving.

class QuickUnion {
  private:
    std::vector<int> P;
    int n_blocks;
  
  public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  QuickUnion(int n): P(std::vector<int> (n, -1)) {
    n_blocks = n;
  };

  // Returns the representative of the class to which i belongs
  int find(int i) {
    if (P[i] < 0) return i;
    else {
      P[i] = find(P[i]);
      return P[i];
    }
  }

  // Performs the union of the classes with representatives ri and rj
  void merge(int i, int j) {
    int ri = find(i);
    int rj = find(j);
    if(ri != rj) {
      if (P[ri] >= P[rj]) {
        // ri is the smallest/shortest
        P[rj] = std::min(P[rj] , P[ri] - 1);
        P[ri] = rj;
        --n_blocks;
      } else {
        // rj is the smallest/shortest
        P[ri] = std::min(P[ri] , P[rj] - 1);
        P[ri] = rj;
        --n_blocks;
      }
    }
  }

  bool connected(int p, int q) {
    return find(p) == find(q);
  }

  // Returns the number of blocks in the union-find set
  int nr_blocks() const {
    return n_blocks;
  };

  void print() {
    for(int num : P) {
      std::cout << num << " ";
    }
    std::cout << std::endl;
  }

};

int main(int argc, char* argv[]) {
  // check if an argument is provided
  if(argc != 2) {
    std::cerr << "Usage: " << argv[1] << "<integer argument>" << std::endl;
    return 1; // return error code
  }
  // convert the argument to an integer
  int n = std::stoi(argv[1]);

  QuickUnion union_obj(n);
  union_obj.print();

  // Generate distinct pairs of elements (i, j) in random order
  std::vector<std::pair<int, int>> pairs;
  for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
          pairs.push_back({i, j});
      }
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(pairs.begin(), pairs.end(), g);

  // Merge pairs until there's only one block left
  int processed_pairs = 0;

  for (const auto& pair : pairs) {
    if (!union_obj.connected(pair.first, pair.second)) {
      union_obj.merge(pair.first, pair.second);
      processed_pairs++;
    }
    if (union_obj.nr_blocks() == 1 || processed_pairs >= n * std::log(n)) {
      break;
    }
  }

  union_obj.print();

  std::cout << "number of blocks: " << union_obj.nr_blocks() << std::endl;
  return 0;
}
