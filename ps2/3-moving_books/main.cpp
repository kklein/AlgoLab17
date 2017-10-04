#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  return 0;
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_friends;
    int n_boxes;
    cin >> n_friends;
    cin >> n_boxes;
    vector<int> strengths(n_friends);
    vector<int> weights(n_boxes);
    for (int i = 0; i < n_friends; i++) {
      cin >> strengths[i];
    }
    for (int i = 0; i < n_boxes; i++) {
      cin >> weights[i];
    }
    sort(strengths.begin(), strengths.end(), greater<int>());
    sort(weights.begin(), weights.end(), greater<int>());

    if (weights[0] > strengths[0]) {
      cout << "impossible" << endl;
    } else {
      int boxes_taken = 0;
      int iterations = 0;
      while (boxes_taken < n_boxes) {
        int friend_index = 0;
        bool progress = true;
        int box_pointer = 0;
        while (friend_index < n_friends and box_pointer < n_boxes) {
          if (weights[box_pointer] > 0) {
            weights[box_pointer] = 0;
            friend_index++;
          }
          box_pointer++;
        }
        iterations++;
      }
      cout << 3 * iterations - 1 << endl;
    }
  }
}
