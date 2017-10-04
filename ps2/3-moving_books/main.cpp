#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_friends;
    int n_boxes;
    cin >> n_friends;
    cin >> n_boxes;
    vector<int> strengths(100, 0);
    vector<int> weights(100, 0);
    int input_strength;
    int max_strength = 0;
    int min_strength = 99;
    for (int i = 0; i < n_friends; i++) {
      cin >> input_strength;
      strengths[input_strength - 1]++;
      max_strength = max(max_strength, input_strength - 1);
      min_strength = min(min_strength, input_strength - 1);
    }
    int input_weight;
    int max_weight = 0;
    int min_weight = 99;
    for (int i = 0; i < n_boxes; i++) {
      cin >> input_weight;
      weights[input_weight - 1]++;
      max_weight = max(max_weight, input_weight - 1);
      min_weight = min(min_weight, input_weight - 1);
    }

    if (max_weight > max_strength) {
      cout << "impossible" << endl;
    } else {
      int boxes_taken = 0;
      int iterations = 0;
      while (boxes_taken < n_boxes) {
	vector<int> friends(100);
	copy(strengths.begin(), strengths.end(), friends.begin());
        int strength_pointer = max_strength;
        int weight_pointer = max_weight;
        while (strength_pointer >= min_strength and weight_pointer >= min_weight) {
          
          if (weights[weight_pointer] <= 0) {
            weight_pointer--;
            //cout << "decrease weight_pointer to: " << weight_pointer << endl;
          } else if (friends[strength_pointer] <= 0) {
            strength_pointer--;
            //cout << "decrease strength_pointer to: " << strength_pointer << endl;
          } else {
            //cout << "else clause" << endl;
            if (weight_pointer <= strength_pointer) {
              weights[weight_pointer]--;
              friends[strength_pointer]--;
              boxes_taken++;
              //cout << "Strength :" << strength_pointer << " ,weight: " <<
              //    weight_pointer << ", new count: " << boxes_taken << endl;
            } else {
              weight_pointer--;
            }
          }
        }
        iterations++;
	//cout << "finished iteration: " << iterations << endl;
      }
      cout << 3 * iterations - 1 << endl;
    }
  }
  return 0;
}
