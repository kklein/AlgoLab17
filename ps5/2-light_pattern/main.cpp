#include<bitset>
#include<iostream>
#include<vector>

using namespace std;

int main() {
  int n_test_cases, n_bulbs, size_pattern, bulb_status, pattern_indicator;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    cin >> n_bulbs >> size_pattern >> pattern_indicator;
    vector<bool> state(n_bulbs);
    for (int bulb_index = 0; bulb_index < n_bulbs; bulb_index++) {
      cin >> bulb_status;
      state[bulb_index] = (bulb_status == 1);
    }
    cin >> pattern_indicator;
    bitset<32> pattern(pattern_indicator);
  }
  return 0;
}
