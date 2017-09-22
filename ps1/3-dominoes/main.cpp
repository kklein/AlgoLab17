#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_dominoes;
    cin >> n_dominoes;
    vector<int> heights(n_dominoes);
    int height;
    for (int i = 0; i < n_dominoes; i++) {
      cin >> height;
      heights[i] = height;
    }
    int domino_pointer = 1;
    int reach_pointer = heights[0];
    int potential_reach;
    bool reachable(true);
    while (reachable && domino_pointer < n_dominoes) {
      if (reach_pointer <= domino_pointer) {
        reachable = false;
      } else {
        reach_pointer = max(reach_pointer, domino_pointer + heights[domino_pointer]);
        domino_pointer++;
      }
    }
    cout << domino_pointer << endl;
  }
  return 0;
}
