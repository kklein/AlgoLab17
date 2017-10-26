#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void test() {
  int n_coins;
  cin >> n_coins;
  vector<int> values(n_coins);
  vector<vector<pair<int, int> > > solution(n_coins, vector<pair<int, int> >(n_coins));
  int value;
  for (int coin = 0; coin < n_coins; coin++) {
    cin >> value;
    values[coin] = value;
    solution[coin][coin] = make_pair(value, 0);
  }
  pair<int, int> p, q;
  int left_value, right_value, priority_value, inferiority_value;
  for (int horizontal_offset = 1; horizontal_offset < n_coins; ++horizontal_offset) {
    for (int row = 0; row < n_coins - horizontal_offset; ++row) {
      p = solution[row][row + horizontal_offset -1];
      q = solution[row + 1][row + horizontal_offset];
      left_value = values[row];
      right_value = values[row + horizontal_offset];
      if (p.second + right_value > q.second + left_value) {
        priority_value = p.second + right_value;
        inferiority_value = p.first;
      } else {
        priority_value = q.second + left_value;
        inferiority_value = q.first;
      }
      solution[row][row + horizontal_offset] = make_pair(priority_value, inferiority_value);
    }
  }
  cout << solution[0][n_coins - 1].first << endl;
}

int main(){
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; ++test_case) {
    test();
  }
  return 0;
}
