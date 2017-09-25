//#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  int n_cards;
  int target_value;
  int sum;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    sum = 0;
    cin >> n_cards;
    cin >> target_value;
    vector<int> cards(n_cards);
    vector<int> sums(n_cards);
    for (int i = 0; i < n_cards; i++) {
      cin >> cards[i];
      sum += cards[i];
      sums[i] = sum;
    } 
    int nearest_result = sums[0];
    int nearest_i = 0;
    int nearest_j = 0;
    for (int j = 1; j < n_cards; j++) {
      if (abs(sums[j] - target_value) < abs(nearest_result - target_value)) {
        nearest_result = sums[j];
        nearest_j = j;
        nearest_i = 0;
      }
    }
    //for (int j = 1; j < n_cards; j++) {
    //  for (int i = 1; i <= j; i++) {
    //    if (abs((sums[j] - sums[i-1]) - target_value) == abs(nearest_result - target_value)) {
    //      if (i < nearest_i) {
    //        nearest_j = j;
    //        nearest_i = i;
    //      } else if (i == nearest_i) {
    //        if (j < nearest_j) {
    //          nearest_j = j;
    //          nearest_i = i;
    //        }
    //     }
    //    } else if (abs((sums[j] - sums[i-1]) - target_value) < abs(nearest_result - target_value)) {
    //     nearest_result = sums[j] - sums[i-1];
    //      nearest_j = j;
    //      nearest_i = i;
    //    }
    //  }
    //}
    for (int i = 1 ; i < n_cards; i++) {
      int goal = target_value - sums[i-1];
      int left = i;
      int right = n_cards;
      bool done = false;
      int middle;
      while (left != right && !done) {
        middle = (right + left) / 2;
        if (sums[middle] < goal) {
          left = middle;
        } else if (sums[middle] > goal) {
          right = middle;
        } else {
          done = true;
        }
      }
        if (abs((sums[middle] - sums[i-1]) - target_value) == abs(nearest_result - target_value)) {
          if (i < nearest_i) {
            nearest_j = middle;
            nearest_i = i;
          } else if (i == nearest_i) {
            if (middle < nearest_j) {
              nearest_j = middle;
              nearest_i = i;
            }
         }
        } else if (abs((sums[middle] - sums[i-1]) - target_value) < abs(nearest_result - target_value)) {
         nearest_result = sums[middle] - sums[i-1];
          nearest_j = middle;
          nearest_i = i;
        }
      
      
    }
    cout << nearest_i << ' ';
    cout << nearest_j << endl;
  }
  return 0;
}
