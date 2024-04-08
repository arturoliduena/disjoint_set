#include <iostream>
#include <random>
#include <vector>

using namespace std;

class QuickFind {
private:
  vector<int> P;
  int n_blocks;

public:
  // Creates the partition {{0}, {1}, ..., {n-1}}
  QuickFind(int n) {
    for (int i = 1; i <= n; ++i) {
      P.push_back(i);
    };
    n_blocks = n;
  };

  // Returns the representative of the class to which
  // i belongs (should be const, but it is not to
  // allow path compression)
  int find(int i) {
    return P[i];
  }

  // Performs the union of the classes with representatives
  // ri and rj, ¿{\color{red} $\text{ri} \not= \text{rj}$}¿
  void merge(int i, int j) {
    int ri = P[i - 1];
    int rj = P[j - 1];
    if (ri != rj) {
      for (int k = 0; k < P.size(); ++k) {
        if (P[k] == rj) {
          P[k] = ri;
        }
      }
      --n_blocks;
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
    for (int num : P) {
      cout << num << " ";
    }
    cout << endl;
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

  QuickFind union_obj(n);
  union_obj.print();

  // Generate distinct pairs of elements (i, j) in random order
  vector<pair<int, int>> pairs;
  for (int i = 1; i <= n; ++i) {
    for (int j = i + 1; j <= n; ++j) {
      pairs.push_back({ i, j });
    }
  }

  random_device rd;
  mt19937 g(rd());
  shuffle(pairs.begin(), pairs.end(), g);

  // Merge pairs until there's only one block left
  int processed_pairs = 0;

  for (const auto& pair : pairs) {
    if (!union_obj.connected(pair.first, pair.second)) {
      union_obj.merge(pair.first, pair.second);
      processed_pairs++;
    }
    if (union_obj.nr_blocks() == 1 || processed_pairs >= n * log(n)) {
      break;
    }
  }

  union_obj.print();

  cout << "number of blocks: " << union_obj.nr_blocks() << endl;
  return 0;
}
